#pragma once

#include "hittable_list.h"
#include "camera.h"
#include "sphere.h"
#include "material.h"
#include "rtweekend.h"
#include "SceneMetadata.h"

#include "vec3.h"

struct SceneMetadata {
    int max_depth;

    float vfov;
    float defocus_angle;
    float focus_dist;

    point3 lookfrom;
    point3 lookat;
    vec3 vup;

    int samples_noisy;
    int samples_clean;
};

int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

void renderToFile(
    hittable_list& world,
    camera& cam,
    const std::string& filename
) {
    std::ofstream file(filename);

    auto old_buffer = std::cout.rdbuf(file.rdbuf());

    cam.render(world);

    std::cout.rdbuf(old_buffer);
}

float randomDouble(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);

    return dist(gen);
}

void saveMetadata(
    const SceneMetadata& m,
    const std::string& filename
) {
    std::ofstream file(filename);

    file << "{\n";

    file << "  \"max_depth\": " << m.max_depth << ",\n";
    file << "  \"vfov\": " << m.vfov << ",\n";
    file << "  \"defocus_angle\": " << m.defocus_angle << ",\n";
    file << "  \"focus_dist\": " << m.focus_dist << ",\n";

    file << "  \"lookfrom\": ["
         << m.lookfrom.x() << ", "
         << m.lookfrom.y() << ", "
         << m.lookfrom.z() << "],\n";

    file << "  \"lookat\": ["
         << m.lookat.x() << ", "
         << m.lookat.y() << ", "
         << m.lookat.z() << "],\n";

    file << "  \"vup\": ["
         << m.vup.x() << ", "
         << m.vup.y() << ", "
         << m.vup.z() << "],\n";

    file << "  \"samples_noisy\": " << m.samples_noisy << ",\n";
    file << "  \"samples_clean\": " << m.samples_clean << "\n";

    file << "}\n";
}

SceneMetadata generateRandomScene(
    hittable_list& world,
    camera& cam
) {
    SceneMetadata metadata;

    // Random material
    float r = randomDouble(0.0, 1.0);
    float g = randomDouble(0.0, 1.0);
    float b = randomDouble(0.0, 1.0);

    auto material = make_shared<lambertian>(colour(r, g, b));

    // Random sphere
    float x = randomDouble(0.0, 100.0);
    float y = randomDouble(0.0, 100.0);
    float z = randomDouble(0.0, 100.0);
    float radius = randomDouble(0.0, 100.0);

    world.add(
        make_shared<sphere>(
            point3(x, y, z),
            radius,
            material
        )
    );

    // Camera
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;

    metadata.max_depth = randomInt(1, 100);
    metadata.vfov = randomDouble(10.0, 100.0);

    metadata.lookfrom = point3(
        randomDouble(0.0, 100.0),
        randomDouble(0.0, 100.0),
        randomDouble(0.0, 100.0)
    );

    metadata.lookat = point3(
        randomDouble(0.0, 100.0),
        randomDouble(0.0, 100.0),
        randomDouble(0.0, 100.0)
    );

    metadata.vup = vec3(0, 1, 0);

    metadata.defocus_angle = randomDouble(0.0, 10.0);
    metadata.focus_dist = randomDouble(1.0, 100.0);

    metadata.samples_noisy = 4;
    metadata.samples_clean = 256;

    // Apply metadata to camera
    cam.max_depth = metadata.max_depth;
    cam.vfov = metadata.vfov;
    cam.lookfrom = metadata.lookfrom;
    cam.lookat = metadata.lookat;
    cam.vup = metadata.vup;
    cam.defocus_angle = metadata.defocus_angle;
    cam.focus_dist = metadata.focus_dist;

    return metadata;
}