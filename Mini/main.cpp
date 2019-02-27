#include "mono.h"

loader::obj_file cube_o("../Res/teapot.obj", loader::obj_face_format::no_backslash);

int main()
{
	// Variables used in the mouse delta, used to 
	// calculate the difference in mouse position from each frame.
	double old_x = 0, old_y = 0;
	double new_x, new_y;
	// Mouse deltas
	double dx, dy;
	
	// View deltas, unused currently, but they're usually used 
	// in the rotation of the camera, 1st person style
	double current_angle_x = 0;
	double current_angle_y = 0;
	
	// Camera is located 1 away in the Z axis from the origin.
	math::vec3 camera_position(0, 0, -1);
	math::vec3 light_direction(0, 1, -1);
	
	// Normalizing the light
	float light_mag = std::sqrtf(light_direction.x_ * light_direction.x_ + 
		light_direction.y_ * light_direction.y_ + light_direction.z_ * light_direction.z_);
	light_direction.x_ / light_mag;
	light_direction.y_ / light_mag;
	light_direction.z_ / light_mag;

	// Object, wrongly named cube here, 
	math::mat4x4 cube_transform = math::mat4x4::translation_matrix({0.0, 0.0, -300.f});
	// Translates the camera position accordingly
	math::mat4x4 camera_transform = math::mat4x4::translation_matrix(camera_position);
	
	// Perspective matrix, check "3d_math.h" for its definition.
	// Was taken off of opengl, transposed to fit my row-major matrices.
	math::mat4x4 g_perspective_matrix = math::mat4x4::perspective_matrix(68.f, 4.f/3.f, 0.01f, 1000.f);
	gl_impl::window g_window({640, 480, ""});

	// Sets up the window, mainly creates the window and sets it
	// as the current OpenGL context
	g_window.setup_window();

	// Placeholder vertices, used extensively w/in the drawing of the object
	math::vec3 projected_1;
	math::vec3 projected_2;	
	math::vec3 projected_3;

	math::vec3 normal;
	math::vec3 line1, line2;

	float normal_mag;

	while (!g_window.should_close())
	{
		g_window.cursor_pos(&new_x, &new_y);
		dx = new_x - old_x;
		dy = new_y - old_y;
		current_angle_x += dx;
		current_angle_y += dy;
		g_window.clear();
		
		/*
		camera_transform = math::mat_mat_multiply(
			math::mat4x4::rotate_x(-1 * (current_angle_x * 0.1f)),
			math::mat4x4::rotate_y(-1 * (current_angle_y * 0.1f))
		);
		*/

		/*
		camera_transform = math::mat_mat_multiply(
			camera_transform,
			math::mat4x4::rotate_x(glfwGetTime())
		);
		*/

		/*
		Migrate the drawing code to something more modern at some point
		*/
		

		for (int i = 0; i < cube_o.get_faces().size(); ++i)
		{

			/*
			Order should always be object transform, then camera transform, then perspective matrix.
			*/
			projected_1 = math::vec_mat_multiply(cube_o.get_faces()[i].point_1, cube_transform);
			projected_1 = math::vec_mat_multiply(projected_1, camera_transform);

			projected_2 = math::vec_mat_multiply(cube_o.get_faces()[i].point_2, cube_transform);
			projected_2 = math::vec_mat_multiply(projected_2, camera_transform);

			projected_3 = math::vec_mat_multiply(cube_o.get_faces()[i].point_3, cube_transform);
			projected_3 = math::vec_mat_multiply(projected_3, camera_transform);


			/*
			Calculating normals
			*/
			line1.x_ = projected_2.x_ - projected_1.x_;
			line1.y_ = projected_2.y_ - projected_1.y_;
			line1.z_ = projected_2.z_ - projected_1.z_;

			line2.x_ = projected_3.x_ - projected_2.x_;
			line2.y_ = projected_3.y_ - projected_2.y_;
			line2.z_ = projected_3.z_ - projected_2.z_;

			// Vector cross product, finds vector perpendicular to
			// the face.
			normal.x_ = line1.y_ * line2.z_ - line1.z_ * line2.y_;
			normal.y_ = line1.z_ * line2.x_ - line1.x_ * line2.z_;
			normal.z_ = line1.x_ * line2.y_ - line1.y_ * line2.x_;

			normal_mag = std::sqrtf(normal.x_ * normal.x_ +
				normal.y_ * normal.y_ +
				normal.z_ * normal.z_);
			// Normalizing each component
			normal.x_ /= normal_mag;
			normal.y_ /= normal_mag;
			normal.z_ /= normal_mag;

			// Do the vector dot product, to basically check if our camera can "see" the
			// current triangle. We can use any point on the triangle for this, so we use
			// the first one.
			if (
				(normal.x_ * (projected_1.x_ - camera_position.x_)
				+ normal.y_ * (projected_1.y_ - camera_position.y_)
				+ normal.z_ * (projected_1.z_ - camera_position.z_)) > 0.f)
			{
				// Light; calculated w/ the dot product of the light direction
				// and the current face
				float dp = normal.x_ * light_direction.x_ +
					normal.y_ * light_direction.y_ +
					normal.z_ * light_direction.z_;


				projected_1 = math::vec_mat_multiply(projected_1, g_perspective_matrix);
				projected_2 = math::vec_mat_multiply(projected_2, g_perspective_matrix);
				projected_3 = math::vec_mat_multiply(projected_3, g_perspective_matrix);

				glBegin(GL_TRIANGLES);
				glColor3f(dp, dp, dp);

				glVertex2f(projected_1.x_, projected_1.y_);
				glVertex2f(projected_2.x_, projected_2.y_);
				glVertex2f(projected_3.x_, projected_3.y_);
				glEnd();
			}

		}


		old_x = new_x;
		old_y = new_y;

		// Flip display and drawing contexts
		g_window.flip();
		// Flush and check window events
		g_window.poll_events();
	}
}
