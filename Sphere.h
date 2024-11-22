#define SPHERE_H
#ifdef SPHERE_H

class Sphere {
public:


	Sphere(sf::Vector2f position_, int radius_, int grain_) 
		: position(position_), radiusX(radius_), radiusY(radius_), grain(grain_) {

		calculateRidge(&ridgeX, radiusX);
		calculateRidge(&ridgeY, radiusY);

		for (int i = 0; i < pow(grain, 2); ++i)
			positionsPoints.push_back(Cord());

		setPositionsPoints();

	}

	Sphere(sf::Vector2f position_, int radius_X_, int radius_Y_, int grain_) 
		: position(position_), radiusX(radius_X_), radiusY(radius_Y_), grain(grain_) {



	}

	void renderRidgePoints(sf::RenderWindow *window) {

		sf::RectangleShape shp;
		shp.setSize(sf::Vector2f(3, 3));

		for (int i = 0; i < ridgeX.size(); ++i) {

			shp.setPosition(sf::Vector2f(position.x + shift(ridgeX[i], radiusX), position.y));
			shp.setFillColor(sf::Color::Red);

			if (ridgeX[i] >= 0 && ridgeX[i] <= std::numbers::pi)
				window->draw(shp);

		}

	}

	void renderPositionsPoints(sf::RenderWindow* window) {

		sf::RectangleShape shp;
		shp.setSize(sf::Vector2f(3, 3));

		for (int i = 0; i < positionsPoints.size(); ++i) {

			shp.setPosition(sf::Vector2f(position.x + positionsPoints[i].getPosition().x, position.y + positionsPoints[i].getPosition().y));
			shp.setFillColor(sf::Color(50*i, 75*i, 25*i));

			if (positionsPoints[i].isVisible)
				window->draw(shp);

		}

	}

	void move(int move) {
		calculateRidge(&ridgeX, radiusX, move);
		setPositionsPoints();
	}
	
private:

	sf::Vector2f position;

	int radiusX, radiusY;
	int grain;

	double shift(double alpha, double radius) {
		return std::cos(alpha) * radius;
	}

	class Cord {
	public:

		Cord(double alpha_, sf::Vector2f position_) {
			setPosition(alpha_, position_);
		};

		Cord() { };

		void setPosition(double alpha_, sf::Vector2f position_) {
			position = position_;
			alpha = alpha_;
			if (alpha > std::numbers::pi) isVisible = false;
			else isVisible = true;
		}

		sf::Vector2f getPosition() {
			return position;
		}

		bool isVisible = false;

	private:

		double alpha;
		sf::Vector2f position;

	};

	std::vector<Cord> positionsPoints;

	std::vector<double> ridgeY;
	std::vector<double> ridgeX;

	void calculateRidge(std::vector<double>* ridge, int radius) {

		double alpha = (std::numbers::pi * 2) / grain;
		double currentAlpha = 0.0;

		for (int i = 0; i <= grain; ++i) {
			currentAlpha += alpha;
			(*ridge).push_back(currentAlpha);
		}

	}

	void calculateRidge(std::vector<double>* ridge, int radius, int move) {

		double alpha = (std::numbers::pi * 2) / grain;
		std::vector<double> newRidge;

		for (int i = 0; i <= grain; ++i) {
			double currentAlpha = (*ridge)[i] + (alpha / 10) * move;
			currentAlpha = normalizeAlpha(currentAlpha);
			newRidge.push_back(currentAlpha);
		}

		*ridge = newRidge;
		
	}

	double normalizeAlpha(double alpha) {

		const double twoPi = std::numbers::pi * 2;
		
		alpha = std::fmod(alpha, twoPi);
		if (alpha < 0) alpha += twoPi;

		return alpha;

	}

	void setPositionsPoints() {

		for (int y = 0; y < grain; ++y)
			for (int x = 0; x < grain; ++x) {
				positionsPoints[y * grain + x].setPosition(ridgeX[x], sf::Vector2f(getEllipseX(radiusX, shift(ridgeX[x], radiusX), shift(ridgeY[y], radiusY)), shift(ridgeY[y], radiusY)));
			}
	}

	double getEllipseX(double radius, double shift, double ellipseY) {
		return (shift / radius) * std::sqrt(std::pow(radius, 2) - std::pow(ellipseY, 2));
	}

};

#endif // SPHERE_H