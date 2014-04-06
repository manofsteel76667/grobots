// GBStringUtilities.h
// Grobots general-purpose string utilities
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBStringUtilities_h
#define GBStringUtilities_h

#include "Platform.h"
#include "Number.h"
#include "FinePoint.h"
#include "Errors.h"
#include "Color.h"

#include <string>
using std::string;

#if MAC
	void ToPascalString(const string & s, Str255 ps);
	string FromPascalString(ConstStr255Param ps);
#endif

string ToString(long n);
string ToString(int n);
string ToString(GBNumber n, int digitsAfterDP = 3, bool trailingZeroes = false);
string ToString(const GBFinePoint & v, int digitsAfterDP = 3, bool trailingZeroes = false);
string ToPercentString(float f, int digitsAfterDP = 1, bool trailingZeroes = false);
#if USE_GBNUMBER
string ToPercentString(GBNumber n, int digitsAfterDP = 1, bool trailingZeroes = false);
#endif
string ToPercentString(long num, long denom, int digitsAfterDP = 1, bool trailingZeroes = false);

bool NamesEquivalent(const string & a, const string & b);

unsigned short HexDigitValue(char digit);
float HexDigitIntensity(char digit);
float HexDigitsIntensity(char d1, char d2);

bool ParseInteger(const string & token, long & number);
bool ParseNumber(const string & token, GBNumber & number);
bool ParseColor(const string & token, GBColor & color);

bool ExtractToken(string & token, const string & line, int & cur);
bool ExtractRest(string & rest, const string & line, int & cur);


#endif