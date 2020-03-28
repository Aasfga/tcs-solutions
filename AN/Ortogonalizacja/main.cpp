#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<vector>
#include<cmath>

#define TYPE long double


class Vector {
	std::vector<TYPE> values;

public:

	Vector() = default;

	Vector(Vector &other) = default;

	TYPE length() {
		return sqrt(dot_product());
	}

	TYPE dot_product() {
		return dot_product(*this);
	}

	TYPE dot_product(Vector &other) {
		TYPE product = 0;
		u_long size = values.size();
		for(int i = 0; i < size; i++)
			product += values[i]*other.values[i];
		return product;
	}

	void operator-=(Vector &other) {
		u_long size = values.size();
		for(int i = 0; i < size; i++)
			values[i] -= other.values[i];
	}

	void sub(TYPE x, Vector &other) {
		u_long size = values.size();

		for(int i = 0; i < size; i++)
			values[i] -= x*other.values[i];
	}

	void normalize() {
		u_long size = values.size();
		TYPE l = length();
		for(int i = 0; i < size; i++) {
			values[i] /= l;
		}
	}

	friend std::ostream &operator<<(std::ostream &out, const Vector &vector) {
		out.precision(10);
		for(auto x : vector.values)
			out<<std::fixed<<x<<" ";
		out<<"\n";
		return out;
	}

	friend std::istream &operator>>(std::istream &in, Vector &vector) {
		for(auto &x : vector.values)
			in>>x;
		return in;
	}

	static Vector *construct_array_of_vectors(int n, int k) {
		auto *vectors = new Vector[k];
		while(k--)
			vectors[k].values.resize(n);
		return vectors;
	}

};


void orthonormalise(Vector *vectors, int k) {
	auto *projections = new TYPE[k]{0};
	for(int i = 0; i < k; i++) {
		for(int j = 0; j < i; j++) {
			projections[j] = vectors[i].dot_product(vectors[j]);
			projections[j] /= vectors[j].dot_product();
		}
		for(int j = 0; j < i; j++) {
			vectors[i].sub(projections[j], vectors[j]);
		}
	}

	for(int i = 0; i < k; i++) {
		vectors[i].normalize();
	}

	delete[] projections;
}


void check(Vector *vectors, int k) {
	for(int i = 0; i < k; i++) {
		for(int j = 0; j < i; j++) {
			std::cout<<"("<<i<<","<<j<<") "<<vectors[i].dot_product(vectors[j])<<"\n";
		}
	}

	for(int i = 0; i < k; i++) {
		std::cout<<i<<": "<<vectors[i].length()<<"\n";
	}
}

int main() {
	int tests;
	std::cin>>tests;

	while(tests--) {
		int n, k;
		std::cin>>n>>k;
		Vector *vectors = Vector::construct_array_of_vectors(n, k);
		for(int i = 0; i < k; i++)
			std::cin>>vectors[i];
		orthonormalise(vectors, k);
		for(int i = 0; i < k; i++)
			std::cout<<vectors[i];

		delete[] vectors;
	}

}
