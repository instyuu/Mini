#pragma once

#include <iostream>

namespace debug {

enum sig
{
	info, warn, err
};

const char *sig_prefix[]
{
	"[DEBUG: INFO] ",
	"[DEBUG: WARN] ",
	"[DEBUG: ERR] ",
};

template<typename First, typename... Rest>
void print(sig protocol, First&& f, Rest&&... r)
{
	std::cout << sig_prefix[protocol] << f;
	// Fold-expression, c++17+
	((std::cout << ", " << r), ...);
	std::cout << "\n";
}

void print_matrix(sig protocol, const math::mat4x4 &m)
{
	std::cout << sig_prefix[protocol] << "Matrix: \n";
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			std::cout << m.c[i][j] << ", " ;
		}
		std::cout << "\n";
	}
}

void print_vector(sig protocol, const math::vec3 &v)
{
	std::cout << sig_prefix[protocol] << v.x_ << ", " << v.y_ << ", " << v.z_ << "\n";
}

}