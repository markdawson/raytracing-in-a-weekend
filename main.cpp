#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

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

void render_image(std::string const& file_name, double i, int width) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = width;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100 + i, -1), 100));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;
    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

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
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            color pixel_color = ray_color(r, world);
            write_color(os, pixel_color);
        }
    }

    fb.close();
    // std::cerr << "\nDone.\n";
}

int main() {

    // Open file
    int image_num = -1;
    for(int ival = -35; ival < 5; ival++) {
        image_num++;

        std::cerr << "\rWriting file: " << image_num << std::flush;

        std::filebuf fb;
        char file_name[100];
        std::snprintf(file_name, 100, "../images/image%04d.ppm", image_num);


        render_image(file_name, std::min(ival / 40.0, -0.125), 1'500);
    }

//      int width = 1'500;
//      render_image("../images/image1.ppm", -0.5, width);
}
