#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <numbers>

class Sphere {
public:
    Sphere(sf::Vector2f position_, int radius_, int grain_)
        : position(position_), radiusX(radius_), radiusY(radius_), grain(grain_) {
        
        calculateRidge(&ridgeX, radiusX);
        calculateRidge(&ridgeY, radiusY);

        positionsPoints.resize((grain_ / 2 + 1) * grain_);
        setPositionsPoints();

        Polygons.resize((grain_ / 2) * grain_, Polygon(position));
        initializePolygons();
    }

    Sphere(sf::Vector2f position_, int radius_X_, int radius_Y_, int grain_)
        : position(position_), radiusX(radius_X_), radiusY(radius_Y_), grain(grain_) {}

    void renderRidgePoints(sf::RenderWindow* window) {
        sf::RectangleShape shp(sf::Vector2f(1, 1));
        shp.setFillColor(sf::Color::Red);

        for (double alpha : ridgeX) {
            shp.setPosition(position.x + shift(alpha, radiusX), position.y);
            if (alpha >= 0 && alpha <= std::numbers::pi) {
                window->draw(shp);
            }
        }
    }

    void renderPositionsPoints(sf::RenderWindow* window) {
        sf::RectangleShape shp(sf::Vector2f(1, 1));
        shp.setFillColor(sf::Color::Red);

        for (const auto& point : positionsPoints) {
            shp.setPosition(position.x + point.getPosition().x, position.y + point.getPosition().y);
            if (point.isVisible) {
                window->draw(shp);
            }
        }
    }

    void renderPolygons(sf::RenderWindow* window) {
        for (auto& polygon : Polygons) {
            polygon.render(window);
        }
    }

    void HorizontalMove(int move) {
        calculateRidge(&ridgeX, radiusX, move);
        setPositionsPoints();
        initializePolygons();
    }

private:
    sf::Vector2f position;
    int radiusX, radiusY, grain;

    double shift(double alpha, double radius) {
        return std::cos(alpha) * radius;
    }

    class Cord {
    public:
        Cord(double alpha_, sf::Vector2f position_) {
            setPosition(alpha_, position_);
        }
        Cord() = default;

        void setPosition(double alpha_, sf::Vector2f position_) {
            position = position_;
            alpha = alpha_;
            isVisible = (alpha <= std::numbers::pi);
        }

        sf::Vector2f getPosition() const {
            return position;
        }

        bool isVisible = false;
    private:
        double alpha;
        sf::Vector2f position;
    };

    class Polygon {
    public:
        Polygon(sf::Vector2f position_) : position(position_) {
            body.setPointCount(4);
            body.setFillColor(sf::Color::Red);
        }

        void setPositionsBodyPoint(const Cord& cordRightDown, const Cord& cordRightUp,
                                   const Cord& cordLeftDown, const Cord& cordLeftUp) {
            if (!cordRightDown.isVisible || !cordRightUp.isVisible ||
                !cordLeftDown.isVisible || !cordLeftUp.isVisible) {
                isVisible = false;
                return;
            }

            body.setPoint(0, position + cordLeftUp.getPosition());
            body.setPoint(1, position + cordRightUp.getPosition());
            body.setPoint(2, position + cordRightDown.getPosition());
            body.setPoint(3, position + cordLeftDown.getPosition());
            isVisible = true;
        }

        void render(sf::RenderWindow* window) {
            if (isVisible) {
                window->draw(body);
            }
        }

        sf::ConvexShape body;
        bool isVisible = false;
    private:
        sf::Vector2f position;
    };

    std::vector<Cord> positionsPoints;
    std::vector<Polygon> Polygons;
    std::vector<double> ridgeY, ridgeX;

    void calculateRidge(std::vector<double>* ridge, int radius) {
        double alpha = (std::numbers::pi * 2) / grain;
        ridge->resize(grain + 1);
        for (int i = 0; i <= grain; ++i) {
            (*ridge)[i] = i * alpha;
        }
    }

    void calculateRidge(std::vector<double>* ridge, int radius, int move) {
        double alpha = (std::numbers::pi * 2) / grain;
        for (double& angle : *ridge) {
            angle = normalizeAlpha(angle + (alpha / 10) * move);
        }
    }

    double normalizeAlpha(double alpha) {
        const double twoPi = std::numbers::pi * 2;
        alpha = std::fmod(alpha, twoPi);
        return (alpha < 0) ? alpha + twoPi : alpha;
    }

    void setPositionsPoints() {
        for (int y = 0; y <= grain / 2; ++y) {
            for (int x = 0; x < grain; ++x) {
                positionsPoints[y * grain + x].setPosition(
                    ridgeX[x],
                    sf::Vector2f(
                        getEllipseX(radiusX, shift(ridgeX[x], radiusX), shift(ridgeY[y], radiusY)),
                        shift(ridgeY[y], radiusY)
                    )
                );
            }
        }
    }

    double getEllipseX(double radius, double shift, double ellipseY) {
        return (shift / radius) * std::sqrt(radius * radius - ellipseY * ellipseY);
    }

    void initializePolygons() {
        for (int y = 0; y < grain / 2; ++y) {
            for (int x = 0; x < grain; ++x) {
                int leftUp    = y * grain + x;
                int rightUp   = y * grain + ((x + 1) % grain);
                int leftDown  = (y + 1) * grain + x;
                int rightDown = (y + 1) * grain + ((x + 1) % grain);

                Polygons[y * grain + x].setPositionsBodyPoint(
                    positionsPoints[rightDown],
                    positionsPoints[rightUp],
                    positionsPoints[leftDown],
                    positionsPoints[leftUp]
                );
            }
        }
    }
};
