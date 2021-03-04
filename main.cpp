#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void render_image(std::string const &file_name, int x, int width) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = width;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    point3 lookfrom(3, 3, 2);
    point3 lootat(0, 0, -1);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom-lootat).length();
    auto aperture = 2.0;

    camera cam(lookfrom, lootat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Opening file
    std::filebuf fb;
    fb.open(file_name, std::ios::out);
    std::ostream os(&fb);

    // std::cerr << "\rWriting file: " << ' ' << std::flush;

    // Render
    os << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
         std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(os, pixel_color, samples_per_pixel);
        }
    }

    fb.close();
    // std::cerr << "\nDone.\n";
}

void make_animation() {
    // Open file
    int image_num = 0;
    for (int ival = 90; ival > 10; ival--) {
        image_num++;

        std::cerr << "\rWriting file: " << image_num << std::flush;

        std::filebuf fb;
        char file_name[100];
        std::snprintf(file_name, 100, "../images/image%04d.ppm", image_num);


        render_image(file_name, ival , 800);
    }
}

int main() {

//    make_animation();

    render_image("../images/image1.ppm", 100, 400);

    return 0;
}
