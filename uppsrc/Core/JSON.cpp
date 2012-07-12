#include "Core.h"

NAMESPACE_UPP

Value ParseJSON(CParser& p)
{
	p.UnicodeEscape();
	if(p.IsDouble())
		return p.ReadDouble();
	if(p.IsString())
		return p.ReadString();
	if(p.Id("null"))
		return Null;
	if(p.Id("true"))
		return true;
	if(p.Id("false"))
		return false;
	if(p.Char('{')) {
		ValueMap m;
		if(!p.IsChar('}'))
			do {
				String key = p.ReadString();
				p.PassChar(':');
				m.Add(key, ParseJSON(p));
			}
			while(p.Char(','));
		p.PassChar('}');
		return m;
	}
	if(p.Char('[')) {
		ValueArray va;
		if(!p.IsChar(']'))
			do
				va.Add(ParseJSON(p));
			while(p.Char(','));
		p.PassChar(']');
		return va;		
	}
	p.ThrowError("Unrecognized JSON element");
	return Null;
}

Value ParseJSON(const char *s)
{
	try {
		CParser p(s);
		return ParseJSON(p);
	}
	catch(CParser::Error) {
		return ErrorValue();
	}
}

String AsJSON(const Value& v, const String& sep, bool pretty)
{
	String r;
	if(v.GetType() == VALUEMAP_V) {
		r << "{";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueMap m = v;
		ValueArray va = m.GetValues();
		for(int i = 0; i < m.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsJSON((String)m.GetKey(i)) << (pretty ? ": " : ":")
			  << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "}";
		return r;
	}
	if(v.GetType() == VALUEARRAY_V) {
		r << "[";
		String sep1;
		if(pretty) {
			r << "\r\n";
			sep1 = sep + '\t';
		}
		ValueArray va = v;
		for(int i = 0; i < va.GetCount(); i++) {
			if(i) {
				r << ",";
				if(pretty)
					r << "\r\n";
			}
			if(pretty)
				r << sep1;
			r << AsJSON(va[i], sep1, pretty);
		}
		if(pretty)
			r << "\r\n" << sep;
		r << "]";
		return r;
	}
	if(IsNumber(v) && IsNull(v))
		return "null";
	if(v.GetType() == INT_V)
		return Format("%d", (int)v);
	if(v.GetType() == BOOL_V)
		return (bool)v ? "true" : "false";
	if(IsNumber(v))
		return Format("%.16g", (double)v);
	if(IsString(v))
		return AsCString((String)v, INT_MAX, NULL, ASCSTRING_OCTALHI|ASCSTRING_JSON);
	if(IsNull(v))
		return "null";
#ifdef SVO_VALUE
	NEVER_("Non-JSON value in AsJSON: " + v.GetTypeName());
#endif
	return "null";
}

String AsJSON(const Value& v, bool pretty)
{
	return AsJSON(v, String(), pretty);
}

template<> void Jsonize(JsonIO& io, double& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsNumber(v)) {
			var = io.Get();
			return;
		}
		if(IsString(v)) {
			double h = ScanDouble((String)v);
			if(!IsNull(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("number expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, int& var)
{
	double v = IntDbl(var);
	Jsonize(io, v);
	if(io.IsLoading())
		if(IsNull(v))
			var = Null;
		else
		if(v >= INT_MIN && v <= INT_MAX && (int)v == v)
			var = (int)v;
		else
			throw JsonizeError("number is not integer");
}

template<> void Jsonize(JsonIO& io, int16& var)
{
	double v = var;
	Jsonize(io, v);
	if(io.IsLoading())
		if(v >= -32768 && v <= 32767 && (int)v == v)
			var = (int16)v;
		else
			throw JsonizeError("16-bit integer expected");
}

template<> void Jsonize(JsonIO& io, bool& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNumber(v) && !IsNull(v))
			var = (bool)v;
		else
			throw JsonizeError("boolean expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, int64& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(v.Is<int64>() || v.Is<int>()) {
			var = v;
			return;
		}
		if(IsNumber(v)) {
			double d = v;
			if(d >= INT64_MIN && d <= INT64_MAX) {
				var = (int64)d;
				return;
			}
		}
		else
		if(IsString(v)) {
			int64 h = ScanInt64((String)v);
			if(!IsNull(h)) {
				var = h;
				return;
			}
		}
		throw JsonizeError("invalid int64 value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
		if(var >= INT_MIN && var <= INT_MAX)
			io.Set(var);
		else
			io.Set(AsString(var));
}

template<> void Jsonize(JsonIO& io, String& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v))
			var = Null;
		else
		if(IsString(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, WString& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v))
			var = Null;
		else
		if(IsString(v))
			var = v;
		else
			throw JsonizeError("string expected");
	}
	else
		io.Set(var);
}

template<> void Jsonize(JsonIO& io, Date& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsString(v)) {
			String text = v;
			if(text.GetCount() > 6) {
				Date d;
				d.year = ScanInt(text.Left(4));
				d.month = ScanInt(text.Mid(4, 2));
				d.day = ScanInt(text.Mid(6));
				if(d.IsValid()) {
					var = d;
					return;
				}
			}
		}
		throw JsonizeError("string expected for Date value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
			io.Set(Format("%04d%02d%02d", var.year, var.month, var.day));
}

template<> void Jsonize(JsonIO& io, Time& var)
{
	if(io.IsLoading()) {
		const Value& v = io.Get();
		if(IsNull(v)) {
			var = Null;
			return;
		}
		if(IsString(v)) {
			String text = v;
			if(text.GetCount() > 15) {
				Time tm;
				tm.year = ScanInt(text.Left(4));
				tm.month = ScanInt(text.Mid(4, 2));
				tm.day = ScanInt(text.Mid(6, 2));
				tm.hour = ScanInt(text.Mid(9, 2));
				tm.minute = ScanInt(text.Mid(12, 2));
				tm.second = ScanInt(text.Mid(15));
				if(tm.IsValid()) {
					var = tm;
					return;
				}
			}
		}
		throw JsonizeError("string expected for Time value");
	}
	else
		if(IsNull(var))
			io.Set(Null);
		else
			io.Set(Format("%04d%02d%02d`T%02d:%02d:%02d",
				          var.year, var.month, var.day, var.hour, var.minute, var.second));
}

String sJsonFile(const char *file)
{
	return file ? String(file) : ConfigFile(GetExeTitle() + ".json");
}

END_UPP_NAMESPACE
