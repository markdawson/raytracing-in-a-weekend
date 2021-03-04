#ifndef RAYTRACING_WEEKEND_CAMERA_H
#define RAYTRACING_WEEKEND_CAMERA_H

class camera {
public:
    camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double vfov, // vertical field of view degree
            double aspect_ratio,
            double aperture,
            double focus_dist
            ) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto focal_length = 1.0;

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif //RAYTRACING_WEEKEND_CAMERA_H
