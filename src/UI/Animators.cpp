#include "oglfv2/UI/Animators.h"

#include <cmath>

#include <glm/glm.hpp>

using namespace UI;

constexpr double PI = 3.1415926535897932384626433;

double EasingFunctions::easeLinear(double x)
{
	return x;
}


double EasingFunctions::easeInSine(double x)
{
	return 1.0 - std::cos((x * PI) / 2.0);
}

double EasingFunctions::easeOutSine(double x)
{
	return std::sin((x * PI) / 2.0);
}

double EasingFunctions::easeInOutSine(double x)
{
	return -(std::cos(PI * x) - 1.0) / 2.0;
}


double EasingFunctions::easeInQuad(double x)
{
	return x * x;
}


double EasingFunctions::easeOutQuad(double x)
{
	return 1.0 - (1.0 - x) * (1.0 - x);
}

double EasingFunctions::easeInOutQuad(double x)
{
	return x < 0.5 ? 2.0 * x * x : 1.0 - std::pow(-2.0 * x + 2.0, 2.0) / 2.0;
}


double EasingFunctions::easeInCubic(double x)
{
	return x * x * x;
}

double EasingFunctions::easeOutCubic(double x)
{
	return 1.0 - std::pow(1.0 - x, 3.0);
}

double EasingFunctions::easeInOutCubic(double x)
{
	return x < 0.5 ? 4.0 * x * x * x : 1.0 - std::pow(-2.0 * x + 2.0, 3.0) / 2.0;
}


double EasingFunctions::easeInQuart(double x)
{
	return x * x * x * x;
}

double EasingFunctions::easeOutQuart(double x)
{
	return 1.0 - std::pow(1.0 - x, 4.0);
}

double EasingFunctions::easeInOutQuart(double x)
{
	return x < 0.5 ? 8.0 * x * x * x * x : 1.0 - std::pow(-2.0 * x + 2.0, 4.0) / 2.0;
}


double EasingFunctions::easeInQuint(double x)
{
	return x * x * x * x * x;
}

double EasingFunctions::easeOutQuint(double x)
{
	return 1.0 - std::pow(1.0 - x, 5.0);
}

double EasingFunctions::easeInOutQuint(double x)
{
	return x < 0.5 ? 16.0 * x * x * x * x * x : 1.0 - pow(-2.0 * x + 2.0, 5.0) / 2.0;
}


double EasingFunctions::easeInExpo(double x)
{
	return x == 0.0 ? 0.0 : std::pow(2.0, 10.0 * x - 10.0);
}

double EasingFunctions::easeOutExpo(double x)
{
	return x == 1.0 ? 1.0 : 1.0 - std::pow(2.0, -10.0 * x);
}

double EasingFunctions::easeInOutExpo(double x)
{
	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: x < 0.5 ? std::pow(2.0, 20.0 * x - 10.0) / 2.0
		: (2.0 - std::pow(2.0, -20.0 * x + 10.0)) / 2.0;
}


double EasingFunctions::easeInCirc(double x)
{
	return 1.0 - std::sqrt(1.0 - std::pow(x, 2.0));
}

double EasingFunctions::easeOutCirc(double x)
{
	return std::sqrt(1.0 - std::pow(x - 1.0, 2.0));
}

double EasingFunctions::easeInOutCirc(double x)
{
	return x < 0.5
		? (1.0 - std::sqrt(1.0 - std::pow(2.0 * x, 2.0))) / 2.0
		: (std::sqrt(1.0 - std::pow(-2.0 * x + 2.0, 2.0)) + 1.0) / 2.0;
}


constexpr double c1 = 1.70158;
constexpr double c2 = c1 * 1.525;
constexpr double c3 = c1 + 1.0;

double EasingFunctions::easeInBack(double x)
{
	return c3 * x * x * x - c1 * x * x;
}

double EasingFunctions::easeOutBack(double x)
{
	return 1.0 + c3 * std::pow(x - 1.0, 3.0) + c1 * std::pow(x - 1.0, 2.0);
}

double EasingFunctions::easeInOutBack(double x)
{
	return x < 0.5
		? (std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0
		: (std::pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) + 2.0) / 2.0;
}


constexpr double c4 = (2.0 * PI) / 3.0;
constexpr double c5 = (2.0 * PI) / 4.5;

double EasingFunctions::easeInElastic(double x)
{
	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: -std::pow(2.0, 10.0 * x - 10.0) * std::sin((x * 10.0 - 10.75) * c4);
}

double EasingFunctions::easeOutElastic(double x)
{
	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: std::pow(2.0, -10.0 * x) * std::sin((x * 10.0 - 0.75) * c4) + 1.0;
}

double EasingFunctions::easeInOutElastic(double x)
{
	return x == 0.0
		? 0.0
		: x == 1.0
		? 1.0
		: x < 0.5
		? -(std::pow(2.0, 20.0 * x - 10.0) * std::sin((20.0 * x - 11.125) * c5)) / 2.0
		: (std::pow(2.0, -20.0 * x + 10.0) * std::sin((20.0 * x - 11.125) * c5)) / 2.0 + 1.0;
}


constexpr double n1 = 7.5625;
constexpr double d1 = 2.75;

double EasingFunctions::easeOutBounce(double x)
{
	if (x < 1.0 / d1) {
		return n1 * x * x;
	}
	else if (x < 2.0 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

double EasingFunctions::easeInBounce(double x)
{
	return 1.0 - easeOutBounce(1.0 - x);
}

double EasingFunctions::easeInOutBounce(double x)
{
	return x < 0.5
		? (1.0 - easeOutBounce(1.0 - 2.0 * x)) / 2.0
		: (1.0 + easeOutBounce(2.0 * x - 1.0)) / 2.0;
}