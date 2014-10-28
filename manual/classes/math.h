/**
  *	\brief Performs common math tasks
  */
class Math
{
public:
	/**
	  * Rounds the number to the closes integer
	  */
	float round(const float value);

	/**
	  * Rounds the number upwards
	  */
	float ceil(const float value);
	
	/**
	  * Rounds the number downwards
	  */
	float floor(const float value);

	/**
	  * Clamps the value between max and min
	  */
	float clamp(const float value, const float min, const float max);
	
	/**
	  * Returns a if a is greater than b, otherwise return b
	  */
	float max(const float a, const float b);
	
	/**
	  * Returns a if a is less than b, otherwise return b
	  */
	float min(const float a, const float b);
	
	/**
	  * Returns the absolute value
	  */
	float abs(const float value);
	
	/**
	  * Returns the square root of value
	  */
	float sqrt(const float value);
	
	/**
	  * Returns a ^ b
	  */
	float pow(const float a, const float b);
	
	/**
	  * Returns a % b
	  * \note This also works for negative values of a and b unlike the mod operator (%)
	  */
	int mod(const int a, const int b);
	
	/**
	  * Returns a >> b
	  * \note This bit-shifting function is circular
	  */
	uint ror(const uint a, const uint b);
	
	/**
	  * Returns a << b
	  * \note This bit-shifting function is circular
	  */
	uint rol(const uint a, const uint b);
	
	/**
	  * Returns (1-t)*a + t*b
	  * \note Expects a t in the range [0, 1]
	  */
	float lerp(const float a, const float b, const float t);
	
	/**
	  * Returns the cosine of x
	  * \param x Angle in radians
	  */
	float cos(const float x);
	
	/**
	  * Returns the sine of x
	  * \param x Angle in radians
	  */
	float sin(const float x);
	
	/**
	  * Returns the tangent of x
	  * \param x Angle in radians
	  */
	float tan(const float x);

	/**
	  * Returns the arccosine of x
	  * \param x Radians
	  */
	float acos(const float x);
	
	/**
	  * Returns the arcsine of x
	  * \param x Radians
	  */
	float asin(const float x);
	
	/**
	  * Returns the arctangent of x
	  * \param x Radians
	  */
	float atan(const float x);

	/**
	  * 
	  */
	float atan2(const float x, const float y);
	
	/**
	  * Seeds the random function 
	  */
	void seedRandom(const uint seed);
	
	/**
	  * Gets a random integer
	  */
	int getRandomInt();
	
	/**
	  * Gets a random integer in the given range
	  */
	int getRandomInt(const int start, const int end);

	/**
	  * Gets a random floating-point number in the range [0, 1]
	  */
	float getRandomFloat();

	/// Floating-point representation of PI
	float PI;
};