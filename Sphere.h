#define SPHERE_H
#ifdef SPHERE_H

class Sphere {

public:


	Sphere(sf::Vector2f position_, int radius_, int grain_) 
		: position(position_), radiusX(radius_), radiusY(radius_), grain(grain_) {

		calculateRidge(&ridgeX, radiusX);
		calculateRidge(&ridgeY, radiusY);

		for (int i = 0; i < (grain_ / 2 + 1) * grain_; ++i)
			positionsPoints.push_back(Cord());

		setPositionsPoints();

		for (int i = 0; i < (grain_ / 2) * grain_; ++i)
			Polygons.push_back(Polygon(position));
		
		initializePolygons();

	}

	Sphere(sf::Vector2f position_, int radius_X_, int radius_Y_, int grain_) 
		: position(position_), radiusX(radius_X_), radiusY(radius_Y_), grain(grain_) {



	}

	void renderRidgePoints(sf::RenderWindow *window) {

		sf::RectangleShape shp;
		shp.setSize(sf::Vector2f(1, 1));
		shp.setFillColor(sf::Color::Red);

		for (int i = 0; i < ridgeX.size(); ++i) {

			shp.setPosition(sf::Vector2f(position.x + shift(ridgeX[i], radiusX), position.y));

			if (ridgeX[i] >= 0 && ridgeX[i] <= std::numbers::pi)
				window->draw(shp);

		}

	}

	void renderPositionsPoints(sf::RenderWindow* window) {

		sf::RectangleShape shp;
		shp.setSize(sf::Vector2f(1, 1));
		shp.setFillColor(sf::Color::Red);

		for (int i = 0; i < positionsPoints.size(); ++i) {

			shp.setPosition(sf::Vector2f(position.x + positionsPoints[i].getPosition().x, position.y + positionsPoints[i].getPosition().y));

			if (positionsPoints[i].isVisible)
				window->draw(shp);

		}

	}

	void renderPolygons(sf::RenderWindow* window) {
		for (auto polygon_ : Polygons)
			polygon_.render(window);
	}

	void HorizontalMove(int move) {
		
		calculateRidge(&ridgeX, radiusX, move);
		setPositionsPoints();

		initializePolygons();

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

	class Polygon {
	public:

		Polygon(sf::Vector2f position_) : position(position_){
			body.setPointCount(4);
			body.setFillColor(sf::Color::Red);
		}

		void setPositionsBodyPoint(Cord cordRightDown, Cord cordRightUp, Cord cordLeftDown, Cord cordLeftUp) {

			if (cordRightDown.isVisible == false) {

			}

			if (cordRightDown.isVisible == true && cordRightUp.isVisible == true
				&& cordLeftDown.isVisible == true && cordLeftUp.isVisible == true) {

				body.setPoint(0, sf::Vector2f(position.x + cordLeftUp.getPosition().x  , position.y + cordLeftUp.getPosition().y));		// Верхний левый
				body.setPoint(3, sf::Vector2f(position.x + cordLeftDown.getPosition().x, position.y + cordLeftDown.getPosition().y));	// Нижний левый


				body.setPoint(1, sf::Vector2f(position.x + cordRightUp.getPosition().x  , position.y + cordRightUp.getPosition().y));   // Верхний правый
				body.setPoint(2, sf::Vector2f(position.x + cordRightDown.getPosition().x, position.y + cordRightDown.getPosition().y));	// Нижний  правый

			}

			if (cordRightDown.isVisible == false || cordRightUp.isVisible == false
				|| cordLeftDown.isVisible == false || cordLeftUp.isVisible == false)
				isVisible = false;
			else if (cordRightDown.isVisible == true && cordRightUp.isVisible == true
				&& cordLeftDown.isVisible == true && cordLeftUp.isVisible == true)
				isVisible = true;

		}

		bool isVisible = false;

		void render(sf::RenderWindow *window) {
			if (isVisible) window->draw(body);
		}

		sf::ConvexShape body;

	private:

		sf::Vector2f position;

	};

	std::vector<Cord> positionsPoints;
	std::vector<Polygon> Polygons;

	std::vector<double> ridgeY;
	std::vector<double> ridgeX;

	void calculateRidge(std::vector<double>* ridge, int radius) {

		double alpha = (std::numbers::pi * 2) / grain;
		double currentAlpha = 0.0;

		for (int i = 0; i <= grain; ++i) {
			(*ridge).push_back(currentAlpha);
			currentAlpha += alpha;
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

		for (int y = 0; y <= grain / 2; ++y)
			for (int x = 0; x < grain; ++x)
				positionsPoints[y * grain + x].setPosition(ridgeX[x], sf::Vector2f(getEllipseX(radiusX, shift(ridgeX[x], radiusX), shift(ridgeY[y], radiusY)), shift(ridgeY[y], radiusY)));

	}

	double getEllipseX(double radius, double shift, double ellipseY) {
		return (shift / radius) * std::sqrt(std::pow(radius, 2) - std::pow(ellipseY, 2));
	}

	void initializePolygons() {

		for (int y = 0; y < grain / 2; ++y) {

			for (int x = 0; x < grain; ++x) {

				int leftUp;
				int rightUp;
				int leftDown;
				int rightDown;

				int colorValue;

				if (x + 1 == grain) {
					leftUp    = (y * grain) + (0 + 0);
					rightUp   = (y * grain) + (x);
					leftDown  = (y + 1) * grain + (0 + 0);
					rightDown = (y + 1) * grain + (x);
				}
				else {
					leftUp = (y * grain) + (x + 0);
					rightUp = (y * grain) + (x + 1);
					leftDown = (y + 1) * grain + (x + 0);
					rightDown = (y + 1) * grain + (x + 1);
				}

				Polygons[y * grain + x].setPositionsBodyPoint(
					positionsPoints[rightDown], // Нижний  правый
					positionsPoints[rightUp],	// Верхний правый
					positionsPoints[leftDown],	// Нижний  левый
					positionsPoints[leftUp]		// Верхний левый
				);

				for (int y = 0; y < grain / 2; ++y) {
					for (int x = 0; x < grain; ++x) {

						float baseLight = (y * 255.0f / (grain / 2)) + 5;

						float noise = 10 * std::sin(x * 0.5f + y * 0.3f);

						int colorValue = static_cast<int>(std::clamp(baseLight + noise, 10.0f, 255.0f));

						Polygons[y * grain + x].body.setFillColor(
							sf::Color(colorValue, colorValue, colorValue)
						);
					}
				}

			}

		}

	}

};

#endif // SPHERE_H