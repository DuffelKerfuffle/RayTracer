#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "SceneMetadata.h"
#include <random>
#include <fstream>
#include <iostream>
#include <filesystem>

// void renderToFile(
//     hittable_list& world,
//     camera& cam,
//     const std::string& filename
// ) {
//     std::ofstream file(filename);

//     auto old_buffer = std::cout.rdbuf(file.rdbuf());

//     cam.render(world);

//     std::cout.rdbuf(old_buffer);
// }


// SceneMetadata generateRandomScene(
//     hittable_list& world,
//     camera& cam
// ) {
//     SceneMetadata metadata;
//     float r = randomNumber(0.0f, 1.0f);
//     float g = randomNumber(0.0f, 1.0f);
//     float b = randomNumber(0.0f, 1.0f);

//     auto material = make_shared<lambertian>(colour(r, g, b));

//     float x = randomNumber(0.0f, 100.0f);
//     float y = randomNumber(0.0f, 100.0f);
//     float z = randomNumber(0.0f, 100.0f);
//     float rad = randomNumber(0.0f, 100.0f);
//     world.add(make_shared<sphere>(point3(x, y, z ), rad));

//     cam.aspect_ratio = 16.0 / 9.0;
//     cam.image_width = 400;

//     int depth = randomNumber(0, 100);
//     cam.max_depth         = depth;
    
//     float vfov = randomNumber(0.0f, 100.0f);
//     cam.vfov     = vfov;
    
//     float lookFromX = randomNumber(0.0f, 100.0f);
//     float lookFromY = randomNumber(0.0f, 100.0f);
//     float lookFromZ = randomNumber(0.0f, 100.0f);
//     cam.lookfrom = point3(lookFromX, lookFromY, lookFromZ);

//     float lookAtX = randomNumber(0.0f, 100.0f);
//     float lookAtY = randomNumber(0.0f, 100.0f);
//     float lookAtZ = randomNumber(0.0f, 100.0f);
    
//     cam.lookat   = point3(lookAtX, lookAtY, lookAtZ);
    
//     cam.vup      = vec3(0,1,0);
    
//     float defocus = randomNumber(0.0f, 100.0f);
//     float focus = randomNumber(0.0f, 100.0f);
//     cam.defocus_angle = defocus;
//     cam.focus_dist    = focus;
    
//     cam.samples_per_pixel = 4;
//     cam.render(world);
//     // saveImage("dataset/4spp/" + std::to_string(id));

//     cam.samples_per_pixel = 256;
//     cam.render(world);
//     // saveImage("dataset/256spp/" + std::to_string(id));
//     return {r, g, b, x, y, z, rad, depth, vfov, lookFromX, lookFromY, lookFromZ, lookAtX, lookAtY, lookAtZ, defocus, focus};
// }

int main() {

    hittable_list world;

    // auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
    // auto material_left   = make_shared<dielectric>(1.50);
    // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    // auto material_right  = make_shared<metal>(colour(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    camera cam;

    for (int i = 0; i < 1000; ++i) {

        std::string dir =
            "dataset/sample_" + std::to_string(i);

        std::filesystem::create_directories(dir);

        hittable_list world;
        camera cam;

        SceneMetadata metadata =
            generateRandomScene(world, cam);

        cam.samples_per_pixel = metadata.samples_noisy;

        renderToFile(
            world,
            cam,
            dir + "/noisy.ppm"
        );

        cam.samples_per_pixel = metadata.samples_clean;

        renderToFile(
            world,
            cam,
            dir + "/clean.ppm"
        );

        saveMetadata(
            metadata,
            dir + "/metadata.json"
        );
    }
}