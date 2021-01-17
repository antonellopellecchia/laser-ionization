#ifndef POINT3D_DEFINITION
#define POINT3D_DEFINITION

class Point3D {
public:
  Point3D();
  Point3D(double x, double y, double z);

  Point3D operator+(const Point3D&);
  Point3D operator-(const Point3D&);
  Point3D operator*(const Point3D&);
  Point3D operator*(const double);
  Point3D& operator*=(const double);

  double x, y, z;
};

#endif