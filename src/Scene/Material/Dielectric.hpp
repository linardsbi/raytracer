//
// Created by hvssz on 04.11.21.
//

class Dielectric : public MaterialBase {
public:
    constexpr explicit Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    [[nodiscard]] constexpr ScatterResult scatter(
            const Ray &r_in, const HitRecord &rec
    ) const override {

        const double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        const auto unit_direction = r_in.direction().unit_vector();

        const double cos_theta = std::fmin(rec.normal.dot(-unit_direction), 1.0);
        const double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

        const bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        const auto direction = cannot_refract || reflectance(cos_theta, refraction_ratio) > random<double>()
                ? unit_direction.reflect(rec.normal)
                : refract(unit_direction, rec.normal, refraction_ratio);

        return {{Ray{rec.p, direction, r_in.time()}}, Color{1.0, 1.0, 1.0}};
    }


private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

    double ir; // Index of Refraction
};