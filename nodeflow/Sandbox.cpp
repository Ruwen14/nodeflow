#include "utility/dbgln.hpp"
#include "3rdparty/tinycolormap/include/tinycolormap.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <array>
#include "reflection/type_reflection.hpp"

using namespace tinycolormap;

namespace nf 
{
	struct Color
	{
		explicit constexpr Color(double gray) noexcept : data{ gray, gray, gray } {}
		constexpr Color(double r, double g, double b) noexcept : data{ r, g, b } {}

		double data[3];

		double& r() noexcept { return data[0]; }
		double& g() noexcept { return data[1]; }
		double& b() noexcept { return data[2]; }
		constexpr double r() const noexcept { return data[0]; }
		constexpr double g() const noexcept { return data[1]; }
		constexpr double b() const noexcept { return data[2]; }

		constexpr uint8_t ri() const noexcept { return static_cast<uint8_t>(data[0] * 255.0); }
		constexpr uint8_t gi() const noexcept { return static_cast<uint8_t>(data[1] * 255.0); }
		constexpr uint8_t bi() const noexcept { return static_cast<uint8_t>(data[2] * 255.0); }

		double& operator[](std::size_t n) noexcept { return data[n]; }
		constexpr double operator[](std::size_t n) const noexcept { return data[n]; }
		double& operator()(std::size_t n) noexcept { return data[n]; }
		constexpr double operator()(std::size_t n) const noexcept { return data[n]; }

		friend constexpr Color operator+(const Color& c0, const Color& c1) noexcept
		{
			return { c0.r() + c1.r(), c0.g() + c1.g(), c0.b() + c1.b() };
		}

		friend constexpr Color operator*(double s, const Color& c) noexcept
		{
			return { s * c.r(), s * c.g(), s * c.b() };
		}

	};

	template<class CharT>
	struct std::formatter<nf::Color, CharT> : std::formatter<CharT> {
		// parse() is inherited from the base class

		// Define format() by calling the base class implementation with the wrapped value
		template<class FormatContext>
		auto format(const nf::Color rgb_value, FormatContext& fc) const {
			return std::format_to(fc.out(), "'rgb({}, {}, {})'", rgb_value.ri(), rgb_value.gi(), rgb_value.bi());
		}
	};


	Color getFromColormap(double x)
	{
		auto lerp = [](double x, const std::vector<Color>& data)
		{
			auto clamp = [](double x) {
				return (x < 0.0) ? 0.0 : (x > 1.0) ? 1.0 : x;
			};

			const double a = clamp(x) * (data.size() - 1);
			const double i = std::floor(a);
			const double t = a - i;
			const Color& c0 = data[static_cast<std::size_t>(i)];
			const Color& c1 = data[static_cast<std::size_t>(std::ceil(a))];

			return (1.0 - t) * c0 + t * c1;
		};

		static const std::vector<Color> data =
		{
			{1, 0.6431372549019608, 0.0196078431372549},
			{1, 1, 0.5019607843137255},
			{1, 0.6588235294117647, 0.7333333333333333},
			{1, 0.8823529411764706, 0},
			{0.5803921568627451, 1, 0.7098039215686275},
			{0.9411764705882353, 0.6392156862745098, 1},
			{0.5607843137254902, 0.48627450980392156, 0},
			{0.6, 0.24705882352941178, 0},
			{0.6, 0, 0},
			{0.8784313725490196, 1, 0.4},
			{0.4549019607843137, 0.0392156862745098, 1},
			{0, 0.3607843137254902, 0.19215686274509805},
			{0.7607843137254902, 0, 0.5333333333333333},
			{0.2980392156862745, 0, 0.3607843137254902},
			{0, 0.2, 0.5019607843137255},
			{0.3686274509803922, 0.9450980392156862, 0.9490196078431372},
			{1, 0.3137254901960784, 0.0196078431372549},
			{0, 0.4588235294117647, 0.8627450980392157},
			{0.615686274509804, 0.8, 0},
			{0.16862745098039217, 0.807843137254902, 0.2823529411764706},
			{0.5019607843137255, 0.5019607843137255, 0.5019607843137255},
			{0.25882352941176473, 0.4, 0},
			{0, 0.6, 0.5607843137254902},
			{1, 0.8, 0.6},
			{1, 0, 0.06274509803921569}
		};

		return lerp(x, data);
	}

}




std::vector<double> createRange(double start, double end, double step)
{
	std::vector<double> range;
	double value = start;
	while (value <= end)
	{
		range.push_back(value);
		value += step;
	}
	return range;
}


nf::Color mapTypeToColorMap(nf::typeid_t typeID)
{
	// normalizes hash between [0.0, 1.0]
	auto normalizeHash = [](nf::typeid_t hash) {
		return static_cast<double>(hash) / static_cast<double>(std::numeric_limits<nf::typeid_t>::max());
	};

	double hashNorm = normalizeHash(typeID);
	dbgln("orign {}, normed {}", typeID, hashNorm);


	return nf::getFromColormap(hashNorm);
}


nf::Color mapTypeToRandomColor(nf::typeid_t typeID)
{	
	static std::mt19937_64 randGen;
	static std::uniform_real_distribution<double> rgb_dist{ 0.0, 1.0 };

	// Reproducible Color for each type
	randGen.seed(typeID);
	return {
		rgb_dist(randGen),
		rgb_dist(randGen),
		rgb_dist(randGen)
	};
}

//  = (x-min(x))/(max(x)-min(x))
long double normalize(std::uint64_t value) {
	return static_cast<double>(value) / static_cast<double>(std::numeric_limits<std::uint64_t>::max());
}

int main() 
{
	std::vector<nf::Color> vec;
	for (size_t i = 0; i < 50; i++)
	{
		vec.push_back(mapTypeToRandomColor(i));
	}

	dbgln(vec);


// 	auto normalized_ = normalize();


	exit(0);
	std::vector<std::string> colors;
	auto range = createRange(0, 1.0, 0.01);


	for (const auto& value : range)
	{
		auto color = nf::getFromColormap(value);

// 		auto color = tinycolormap::GetColor(value, tinycolormap::ColormapType::Distinct);
		colors.push_back(std::format("'rgb({}, {}, {})'", color.ri(), color.gi(), color.bi()));
	}

	dbgln(colors);

	return 0;
}