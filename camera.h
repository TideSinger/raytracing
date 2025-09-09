#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera
{
public:

	double aspect_ratio = 1.0; // ratio of l/w
	int image_width = 100; //width in pixels
	int samples_per_pixel = 10; //random samples per pixel
	int max_depth = 10; //max ray bounces in a scene

	double vfov = 90; //vertical FOV
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);

	double defocus_angle = 0; //angle variation
	double focus_dist = 10; //distance to perfect focus

	void render(const hittable& world) {
		// ready variables and scene
		initialize();

		//render that shi
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++){
					ray r = get_ray(i, j);
					pixel_color += ray_color(r,max_depth, world);
				}
				write_color(std::cout, pixel_samples_scale*pixel_color);
			}
		}
		std::clog << "\rDone.                 \n";
	}
	
private:

	int image_height; // img height
	double pixel_samples_scale; //color factor per sample
	point3 center; // center of camera
	point3 pixel00_loc; // 0,0 pixel location
	vec3 pixel_delta_u; // pixel offset right
	vec3 pixel_delta_v; // pixel offset down
	vec3 u, v, w;	// Camera basis
	vec3 defocus_disc_u; //defocus horiz rad
	vec3 defocus_disc_v; //defocus vertical rad

	void initialize() {
		
		// Image Information
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = lookfrom;


		// Calculate the image height, and ensure that it's at least 1.
		auto theta = degrees_to_radians(vfov);
		auto h = std::tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		auto viewport_width = viewport_height * (double(image_width) / image_height);
		
		// Calculate basis vectors for the adjusted camera coordinates

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		auto viewport_u = viewport_width * u;
		auto viewport_v = viewport_height * -v;

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	
		auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disc_u = u * defocus_radius;
		defocus_disc_v = v * defocus_radius;
	}

	ray get_ray(int i, int j) const {
		//build camera rays from defocus disc at random point around i,j

		auto offset = sample_square();

		auto pixel_sample = pixel00_loc
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);
		auto ray_origin = (defocus_angle <= 0) ? center : defocus_disc_sample();
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		// make a vec3 of a random area +- 0.5
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	point3 defocus_disc_sample() const {
		//random point in the camera disc
		auto p = random_in_unit_disk();
		return center + (p[0] * defocus_disc_u) + (p[1] * defocus_disc_v);
	}

	color ray_color(const ray& r,int depth, const hittable & world) const{
		
		if (depth <= 0) {// we hit rock bottom
			return color(0, 0, 0);
		}

		hit_record rec;

		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return color(0, 0, 0);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * unit_direction.y() + 1;
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};

#endif // !CAMERA_H
