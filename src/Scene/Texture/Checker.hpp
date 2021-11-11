//
// Created by hvssz on 10.11.21.
//

#pragma once

class Checker : public Texture {
public:
    Checker(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
            : odd(std::move(_odd)), even(std::move(_even)) {}

    Checker(const Color &c1, const Color &c2)
            : odd(std::make_shared<SolidColor>(c2)), even(std::make_shared<SolidColor>(c1)) {}

    [[nodiscard]] Color value(double u, double v, const Point3 &p) const override {
        const auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        }
        return even->value(u, v, p);
    }

private:
    std::shared_ptr <Texture> odd;
    std::shared_ptr <Texture> even;
};
