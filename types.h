#ifndef CIM_TYPES_H_INC
#define CIM_TYPES_H_INC

#include <string.h>
#include <string>

const size_t CIM_MAXLEN = 1024;

enum cimReturn {
	/** Indicates that a function was successful */
	cimReturn_SUCCESS = 0x0,

	/** Indicates that a function failed */
	cimReturn_FAILURE = -0x1,

	/** Indicates that not enough memory was available
	 * to perform the requested operation
	 */
	cimReturn_OUTOFMEMORY = -0x3,

	/** @cond never
	 *  Force 32-bit size enum
	 */
	_CIM_ENFORCE_ENUM_SIZE = 0x7fffffff
};  // !enum aiReturn

// just for backwards compatibility, don't use these constants anymore
#define CIM_SUCCESS     cimReturn_SUCCESS
#define CIM_FAILURE     cimReturn_FAILURE
#define CIM_OUTOFMEMORY cimReturn_OUTOFMEMORY

// ----------------------------------------------------------------------------------
/** Seek origins (for the virtual file system API).
 *  Much cooler than using SEEK_SET, SEEK_CUR or SEEK_END.
 */
enum cimOrigin {
	/** Beginning of the file */
	cimOrigin_SET = 0x0,

	/** Current position of the file pointer */
	cimOrigin_CUR = 0x1,

	/** End of the file, offsets must be negative */
	cimOrigin_END = 0x2,

	/**  @cond never
	 *   Force 32-bit size enum
	 */
	_CIM_ORIGIN_ENFORCE_ENUM_SIZE = 0x7fffffff
}; // !enum aiOrigin

struct cimString {
#ifdef __cplusplus
	/** Default constructor, the string is set to have zero length */
	cimString() :
		length(0) {
		data[0] = '\0';

#ifdef _DEBUG
		// Debug build: overwrite the string on its full length with ESC (27)
		memset(data + 1, 27, CIM_MAXLEN - 1);
#endif
	}

	/** Copy constructor */
	cimString(const cimString& rOther) :
		length(rOther.length) {
		// Crop the string to the maximum length
		length = length >= CIM_MAXLEN ? CIM_MAXLEN - 1 : length;
		memcpy(data, rOther.data, length);
		data[length] = '\0';
	}

	/** Constructor from std::string */
	explicit cimString(const std::string& pString) :
		length(pString.length()) {
		length = length >= CIM_MAXLEN ? CIM_MAXLEN - 1 : length;
		memcpy(data, pString.c_str(), length);
		data[length] = '\0';
	}

	/** Copy a std::string to the aiString */
	void Set(const std::string& pString) {
		if (pString.length() > CIM_MAXLEN - 1) {
			return;
		}
		length = pString.length();
		memcpy(data, pString.c_str(), length);
		data[length] = 0;
	}

	/** Copy a const char* to the aiString */
	void Set(const char* sz) {
		const size_t len = ::strlen(sz);
		if (len > CIM_MAXLEN - 1) {
			return;
		}
		length = len;
		memcpy(data, sz, len);
		data[len] = 0;
	}

	/** Assign a const char* to the string */
	cimString& operator = (const char* sz) {
		Set(sz);
		return *this;
	}

	/** Assign a cstd::string to the string */
	cimString& operator = (const std::string& pString) {
		Set(pString);
		return *this;
	}

	/** Comparison operator */
	bool operator==(const cimString& other) const {
		return  (length == other.length && 0 == memcmp(data, other.data, length));
	}

	/** Inverse comparison operator */
	bool operator!=(const cimString& other) const {
		return  (length != other.length || 0 != memcmp(data, other.data, length));
	}

	/** Append a string to the string */
	void Append(const char* app) {
		const size_t len = strlen(app);
		if (!len) {
			return;
		}
		if (length + len >= CIM_MAXLEN) {
			return;
		}

		memcpy(&data[length], app, len + 1);
		length += len;
	}

	/** Clear the string - reset its length to zero */
	void Clear() {
		length = 0;
		data[0] = '\0';

#ifdef _DEBUG
		// Debug build: overwrite the string on its full length with ESC (27)
		memset(data + 1, 27, CIM_MAXLEN - 1);
#endif
	}

	/** Returns a pointer to the underlying zero-terminated array of characters */
	const char* C_Str() const {
		return data;
	}

#endif // !__cplusplus

	/** Binary length of the string excluding the terminal 0. This is NOT the
	 *  logical length of strings containing UTF-8 multibyte sequences! It's
	 *  the number of bytes from the beginning of the string to its end.*/
	size_t length;

	/** String buffer. Size limit is MAXLEN */
	char data[CIM_MAXLEN];
};  // !struct aiString

#pragma pack(push, 1)
struct cimColor3D {
#ifdef __cplusplus
	cimColor3D() : r(0.0f), g(0.0f), b(0.0f) {}
	cimColor3D(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	cimColor3D(float _r) : r(_r), g(_r), b(_r) {}
	cimColor3D(const cimColor3D& o) : r(o.r), g(o.g), b(o.b) {}

	/** Component-wise comparison */
	// TODO: add epsilon?
	bool operator == (const cimColor3D& other) const {
return r == other.r && g == other.g && b == other.b;}

			/** Component-wise inverse comparison */
			// TODO: add epsilon?
			bool operator != (const cimColor3D& other) const 				{
return r != other.r || g != other.g || b != other.b;}

					/** Component-wise addition */
			cimColor3D operator+(const cimColor3D& c) const {
						return cimColor3D(r + c.r, g + c.g, b + c.b);
					}

					/** Component-wise subtraction */
					cimColor3D operator-(const cimColor3D& c) const {
						return cimColor3D(r - c.r, g - c.g, b - c.b);
					}

					/** Component-wise multiplication */
					cimColor3D operator*(const cimColor3D& c) const {
						return cimColor3D(r*c.r, g*c.g, b*c.b);
					}

					/** Multiply with a scalar */
					cimColor3D operator*(float f) const {
						return cimColor3D(r*f, g*f, b*f);
					}

					/** Access a specific color component */
					float operator[](unsigned int i) const {
						return *(&r + i);
					}

					/** Access a specific color component */
					float& operator[](unsigned int i) {
						return *(&r + i);
					}

					/** Check whether a color is black */
					bool IsBlack() const {
						static const float epsilon = 10e-3f;
						return fabs(r) < epsilon && fabs(g) < epsilon && fabs(b) < epsilon;
					}

#endif // !__cplusplus

					//! Red, green and blue color values
					float r, g, b;
};



#pragma pack(push, 1)
struct cimColor4D {
#ifdef __cplusplus
	cimColor4D() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	cimColor4D(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	cimColor4D(float _r) : r(_r), g(_r), b(_r), a(_r) {}
	cimColor4D(const cimColor4D& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}

	/** Component-wise comparison */
	// TODO: add epsilon?
	bool operator == (const cimColor4D& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	/** Component-wise inverse comparison */
	// TODO: add epsilon?
	bool operator != (const cimColor4D& other) const {
		return r != other.r || g != other.g || b != other.b  || a != other.a;
	}

	/** Component-wise addition */
	cimColor4D operator+(const cimColor4D& c) const {
		return cimColor4D(r + c.r, g + c.g, b + c.b, a + c.a);
	}

	/** Component-wise subtraction */
	cimColor4D operator-(const cimColor4D& c) const {
		return cimColor4D(r - c.r, g - c.g, b - c.b, a - c.a);
	}

	/** Component-wise multiplication */
	cimColor4D operator*(const cimColor4D& c) const {
		return cimColor4D(r*c.r, g*c.g, b*c.b, a*c.a);
	}

	/** Multiply with a scalar */
	cimColor4D operator*(float f) const {
		return cimColor4D(r*f, g*f, b*f, a*f);
	}

	/** Access a specific color component */
	float operator[](unsigned int i) const {
		return *(&r + i);
	}

	/** Access a specific color component */
	float& operator[](unsigned int i) {
		return *(&r + i);
	}

	/** Check whether a color is black */
	bool IsBlack() const {
		static const float epsilon = 10e-3f;
		return fabs(r) < epsilon && fabs(g) < epsilon && fabs(b) < epsilon && fabs(a) < epsilon;
	}

#endif // !__cplusplus

	//! Red, green and blue color values
	float r, g, b, a;
};
#endif