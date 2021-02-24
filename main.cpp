#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>


color ray_color(const ray &r, const hittable &world) {
    hit_record rec;

    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

void render_image(std::string const &file_name, double x, int width) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = width;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, x), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Opening file
    std::filebuf fb;
    fb.open(file_name, std::ios::out);
    std::ostream os(&fb);

    // std::cerr << "\rWriting file: " << ' ' << std::flush;

    // Render
    os << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        // std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(os, pixel_color, samples_per_pixel);
        }
    }

    fb.close();
    // std::cerr << "\nDone.\n";
}

int main() {

    // Open file
    int image_num = -1;
    for (int ival = -10; ival < 0; ival++) {
        image_num++;

        std::cerr << "\rWriting file: " << image_num << std::flush;

        std::filebuf fb;
        char file_name[100];
        std::snprintf(file_name, 100, "../images/image%04d.ppm", image_num);


        render_image(file_name, ival / 10.0, 600);
    }

//      int width = 1'500;
//      render_image("../images/image1.ppm", -0.5, 1'400);

    return 0;
}
