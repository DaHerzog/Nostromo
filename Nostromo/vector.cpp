#include "vector.h"

#define EPSILON 1e-6

using namespace std;


Vector::Vector(float x, float y, float z) {
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector() {

}

float Vector::dot(const Vector& v) const {
    float tmp = (this->X * v.X) + (this->Y * v.Y) + (this->Z * v.Z);
    return tmp;
}

Vector Vector::cross( const Vector& v) const {
    Vector tmp = Vector(this->Y * v.Z - v.Y * this->Z, this->Z * v.X - v.Z * this->X, this->X * v.Y - v.X * this->Y);
    return tmp;
}

Vector Vector::operator+(const Vector& v) const {
    Vector tmp = Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
    return tmp;
}

Vector Vector::operator-(const Vector& v) const {
    Vector tmp = Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
    return tmp;
}

Vector& Vector::operator+=(const Vector& v) {
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this;
}

Vector Vector::operator*(float c) const {
    Vector tmp = Vector(this->X * c, this->Y * c, this->Z * c);
    return tmp;
}

Vector Vector::operator-() const {
    Vector tmp = Vector(this->X * -1, this->Y * -1, this->Z * -1);
    return tmp;
}

Vector& Vector::normalize() {
    float tmp = this->length();
    this->X /= tmp;
    this->Y /= tmp;
    this->Z /= tmp;
    return *this;
}

float Vector::length() const {
    float tmp = sqrt(this->lengthSquared());
    return tmp;
}

float Vector::lengthSquared() const {
    float tmp = this->X * this->X + this->Y * this->Y + this->Z * this->Z;
    return tmp;
}

Vector Vector::reflection(const Vector& normal) const {
    //formula: x- 2*((s*x)/(s*s))*s  with s: spiegelnormale
    float factor = 2* (normal.dot(*this)/ normal.dot(normal));
    Vector tmp = Vector(this->X - (normal.X * factor),this->Y - (normal.Y * factor),this->Z - (normal.Z * factor));
    return tmp;

}

bool Vector::triangleIntersection( const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {

    //normale der ebene berechnen
    Vector n = ((b - a).cross(c - a)).normalize();
	float faktor = n.dot(a); 
	s = (faktor - n.dot(*this)) / n.dot(d);
	Vector schnitt = *this + (d * s);

    float sizeA = ((c - a).cross(b - a)).length();
    float sizeB = ((b - a).cross(schnitt - a)).length() + ((c - a).cross(schnitt - a)).length() + ((c - b).cross(schnitt - b)).length();


	//float differenz = sizeA - sizeB;

    if((sizeA + 0.0001) > sizeB && s>0) {
        return true;
    } else {
        return false;
    }
}

void Vector::toString() {
    std::cout << this->X << "-" << this->Y << "-" << this->Z << std::endl;
}
