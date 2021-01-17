#include "Point3D.hh"

Point3D::Point3D() {}

Point3D::Point3D(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Point3D Point3D::operator+(const Point3D &point) {
  return Point3D(this->x+point.x, this->y+point.y, this->z+point.z);
}

Point3D Point3D::operator-(const Point3D &point) {
  return Point3D(this->x-point.x, this->y-point.y, this->z-point.z);
}

Point3D Point3D::operator*(const Point3D &point) {
  return Point3D(this->x*point.x, this->y*point.y, this->z*point.z);
}

Point3D Point3D::operator*(const double k) {
  return Point3D(this->x*k, this->y*k, this->z*k);
}

Point3D& Point3D::operator*=(const double k) {
  this->x*=k;
  this->y*=k; 
  this->z*=k;
  return *this;
}