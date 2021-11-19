#include "Platform/Platform.hpp"

//Data Structures Used:
//1)Vectors;
//2)Threads;
//3)Arrays;

/*
#include <SFML/Graphics.hpp>
#include <cmath>

const int IMAGE_WIDTH = 1000;
const int IMAGE_HEIGHT = 600;
double zoom = 0.004; // allow the user to zoom in and out...
double offsetX = -0.7; // and move around
double offsetY = 0.0;
const int MAX = 127; // maximum number of iterations for mandelbrot()

int mandelbrot(double, double, int);
sf::Color getColor(int);

int main() {
    sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Mandelbrot");
    window.setFramerateLimit(30);

    sf::Image image;
    image.create(IMAGE_WIDTH, IMAGE_HEIGHT, sf::Color(0, 0, 0));
    sf::Texture texture;
    sf::Sprite sprite;

    bool stateChanged = true; // track whether the image needs to be regenerated

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    stateChanged = true; // image needs to be recreated when the user changes zoom or offset
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            window.close();
                            break;
                        case sf::Keyboard::Equal:
                            zoom *= 0.9;
                            break;
                        case sf::Keyboard::Dash:
                            zoom /= 0.9;
                            break;
                        case sf::Keyboard::W:
                            offsetY -= 40 * zoom;
                            break;
                        case sf::Keyboard::S:
                            offsetY += 40 * zoom;
                            break;
                        case sf::Keyboard::A:
                            offsetX -= 40 * zoom;
                            break;
                        case sf::Keyboard::D:
                            offsetX += 40 * zoom;
                            break;
                        default: break;
                    }
                default:
                    break;
            }
        }

        if (stateChanged) { // only generate a new image if something has changed, to avoid unnecessary lag
            for (int x = 0; x < IMAGE_WIDTH; x++) {
                for (int y = 0; y < IMAGE_HEIGHT; y++) {
                    // convert x and y to the appropriate complex number
                    double real = (x - IMAGE_WIDTH / 2.0) * zoom + offsetX;
                    double imag = (y - IMAGE_HEIGHT / 2.0) * zoom + offsetY;
                    int value = mandelbrot(real, imag, MAX);
                    image.setPixel(x, y, getColor(value));
                }
            }
            texture.loadFromImage(image);
            sprite.setTexture(texture);
        }

        window.clear();
        window.draw(sprite);
        window.display();

        stateChanged = false;
    }

    return 0;
}

int mandelbrot(double startReal, double startImag, int maximum) {
    int counter = 0;
    double zReal = startReal;
    double zImag = startImag;
    double nextRe;

    while (pow(zReal, 2.0) + pow(zImag, 2.0) <= 4.0 && counter <= maximum) {
        nextRe = pow(zReal, 2.0) - pow(zImag, 2.0) + startReal;
        zImag = 2.0 * zReal * zImag + startImag;
        zReal = nextRe;
        if (zReal == startReal && zImag == startImag) { // a repetition indicates that the point is in the Mandelbrot set
            return -1; // points in the Mandelbrot set are represented by a return value of -1
        }
        counter += 1;
    }

    if (counter >= maximum) {
        return -1; // -1 is used here to indicate that the point lies within the Mandelbrot set
    } else {
        return counter; // returning the number of iterations allows for colouring
    }
}

sf::Color getColor(int iterations) {
    int r, g, b;

    if (iterations == -1) {
        r = 0;
        g = 0;
        b = 0;
    } else if (iterations == 0) {
        r = 255;
        g = 0;
        b = 0;
    } else {
        // colour gradient:      Red -> Blue -> Green -> Red -> Black
        // corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
        if (iterations < 16) {
            r = 16 * (16 - iterations);
            g = 0;
            b = 16 * iterations - 1;
        } else if (iterations < 32) {
            r = 0;
            g = 16 * (iterations - 16);
            b = 16 * (32 - iterations) - 1;
        } else if (iterations < 64) {
            r = 8 * (iterations - 32);
            g = 8 * (64 - iterations) - 1;
            b = 0;
        } else { // range is 64 - 127
            r = 255 - (iterations - 64) * 4;
            g = 0;
            b = 0;
        }
    }

    return sf::Color(r, g, b);
}
*/

#include <SFML/Graphics.hpp>
#include <array>
#include <thread>
#include <vector>

#define ZOOM_FACTOR 0.9;
#define OFFSET_FACTOR 40;

static constexpr int IMAGE_WIDTH = 1000;
static constexpr int IMAGE_HEIGHT = 600;

class Mandelbrot
{
public:
	Mandelbrot();
	//Constructor

	void updateImage(double zoom, double offsetX, double offsetY, sf::Image& image) const;
	//Updating the image based on zoom

	double zoom_in(double zoom);
	//Used to Update the value of zoom so as to render zoomed in picture;

	double zoom_out(double zoom);
	//Used to Update the value of zoom so as to render zoomed out picture;

	double move_up(double zoom, double offsetY);
	//Used to Update the value of offsetY so as to render moved up picture;

	double move_down(double zoom, double offsetY);
	//Used to Update the value of offsetY so as to render moved down picture;

	double move_left(double zoom, double offsetX);
	//Used to Update the value of offsetX so as to render moved left picture;

	double move_right(double zoom, double offsetX);
	//Used to Update the value of offsetX so as to render moved right picture;

	void updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY) const;
	//Generating or updating the image size while the offsets or zoom is being changed by the keys pressed;

private:
	static const int MAX = 127;
	// maximum number of iterations for mandelbrot()

	std::array<sf::Color, MAX + 1> colors;

	int mandelbrot(double startReal, double startImag) const;
	//Computing mandelbrot iterations value

	sf::Color getColor(int iterations) const;
	//Generating color based on iterations
};

Mandelbrot::Mandelbrot()
{
	for (int i = 0; i <= MAX; ++i)
	{
		colors[i] = getColor(i);
	}
}

// Manderbrot Algorithm:
// Find the number of iterations
// of the function f(z)=z*z+c for which |f(z)|>2
// return the number of iterations

int Mandelbrot::mandelbrot(double startReal, double startImag) const
{
	double zReal = startReal;
	double zImag = startImag;

	for (int counter = 0; counter < MAX; ++counter)
	{
		double r2 = zReal * zReal;
		double i2 = zImag * zImag;

		if (r2 + i2 > 4.0)
		{
			return counter;
		}

		zImag = 2.0 * zReal * zImag + startImag;
		zReal = r2 - i2 + startReal;
	}
	return MAX;
}

double Mandelbrot::zoom_in(double zoom)
{

	double zoomed_in;
	zoomed_in = zoom * ZOOM_FACTOR;
	return zoomed_in;
}

double Mandelbrot::zoom_out(double zoom)
{

	double zoomed_out;
	zoomed_out = zoom / ZOOM_FACTOR;
	return zoomed_out;
}

double Mandelbrot::move_up(double zoom, double offsetY)
{

	double moved_up;
	moved_up = offsetY - zoom * OFFSET_FACTOR;
	return moved_up;
}

double Mandelbrot::move_down(double zoom, double offsetY)
{

	double moved_down;
	moved_down = offsetY + zoom * OFFSET_FACTOR;
	return moved_down;
}

double Mandelbrot::move_left(double zoom, double offsetX)
{

	double moved_left;
	moved_left = offsetX - zoom * OFFSET_FACTOR;
	return moved_left;
}

double Mandelbrot::move_right(double zoom, double offsetX)
{

	double moved_right;
	moved_right = offsetX + zoom * OFFSET_FACTOR;
	return moved_right;
}

sf::Color Mandelbrot::getColor(int iterations) const
{
	int r, g, b;

	// colour gradient:      Red -> Blue -> Green -> Red -> Black
	// corresponding values:  0  ->  16  ->  32   -> 64  ->  127 (or -1)
	if (iterations < 16)
	{ //range is 0 - 16
		r = 16 * (16 - iterations);
		g = 0;
		b = 16 * iterations - 1;
	}

	else if (iterations < 32)
	{ //range is 16 - 32
		r = 0;
		g = 16 * (iterations - 16);
		b = 16 * (32 - iterations) - 1;
	}

	else if (iterations < 64)
	{ //range is 32 - 64
		r = 8 * (iterations - 32);
		g = 8 * (64 - iterations) - 1;
		b = 0;
	}

	else
	{ //range is 64 - 127
		r = 255 - (iterations - 64) * 4;
		g = 0;
		b = 0;
	}

	return sf::Color(r, g, b);
	//returns respective shade based on the iterations
}

void Mandelbrot::updateImageSlice(double zoom, double offsetX, double offsetY, sf::Image& image, int minY, int maxY) const
{

	double real = 0 * zoom - IMAGE_WIDTH / 2.0 * zoom + offsetX;
	double imagstart = minY * zoom - IMAGE_HEIGHT / 2.0 * zoom + offsetY;
	for (int x = 0; x < IMAGE_WIDTH; x++, real += zoom)
	{
		double imag = imagstart;
		for (int y = minY; y < maxY; y++, imag += zoom)
		{
			int value = mandelbrot(real, imag);
			image.setPixel(x, y, colors[value]);
		}
	}
}

//This Function is to maximise number of operations whih are being done
//Since there are many requests being made each update image slice is stored in a thread;
//And they will be joined at the end.

//This well help us to improve the running time of the code

/*

void Mandelbrot::updateImage(double zoom, double offsetX, double offsetY, sf::Image& image) const
{
	const int STEP = IMAGE_HEIGHT / std::thread::hardware_concurrency();
	std::vector<std::thread> threads;

	for (int i = 0; i < IMAGE_HEIGHT; i += STEP)
	{
		threads.push_back(std::thread(&Mandelbrot::updateImageSlice, *this, zoom, offsetX, offsetY, std::ref(image), i, std::min(i + STEP, IMAGE_HEIGHT)));
	}

	for (auto& t : threads)
	{
		t.join();
	}
}
*/

int main()
{

	double offsetX = -1;
	double offsetY = 0.0;
	double zoom = 0.005; // allow the user to zoom in and out...

	Mandelbrot mb;

	sf::RenderWindow window(sf::VideoMode(IMAGE_WIDTH, IMAGE_HEIGHT), "Mandelbrot");
	window.setFramerateLimit(0);

	sf::Image image;
	image.create(IMAGE_WIDTH, IMAGE_HEIGHT, sf::Color(0, 0, 0));
	sf::Texture texture;
	sf::Sprite sprite;

	bool stateChanged = true; // track whether the image needs to be regenerated

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					stateChanged = true; // image needs to be recreated when the user changes zoom or offset
					switch (event.key.code)
					{

						case sf::Keyboard::Escape: //press esc key to close
							window.close();
							break;

						case sf::Keyboard::Equal: //press = or + key to zoom in
							zoom = mb.zoom_in(zoom);
							break;

						case sf::Keyboard::Dash: //press - key to zoom out
							zoom = mb.zoom_out(zoom);
							break;

						case sf::Keyboard::W: //press W key to move up
							offsetY = mb.move_up(zoom, offsetY);
							break;

						case sf::Keyboard::S: //press S key to move down
							offsetY = mb.move_down(zoom, offsetY);
							break;

						case sf::Keyboard::A: //press A key to move left
							offsetX = mb.move_left(zoom, offsetX);
							break;

						case sf::Keyboard::D: //press D kry to move right
							offsetX = mb.move_right(zoom, offsetX);
							break;

						default:
							stateChanged = false;
							break;
					}
				default:
					break;
			}
		}

		if (stateChanged)
		{

			mb.updateImageSlice(zoom, offsetX, offsetY, image, 0, IMAGE_HEIGHT);
			texture.loadFromImage(image);
			sprite.setTexture(texture);
			stateChanged = false;
		}
		window.draw(sprite);
		window.display();
	}
}
