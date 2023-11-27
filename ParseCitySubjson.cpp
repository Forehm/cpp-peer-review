struct ParseCitySubjsonRequest {
    const string& country_name;
    const string& country_iso_code;
    const string& country_phone_code;
    const string& country_time_zone;
    const vector<Language>& languages;
};

void ParseCitySubjson(vector<City>& cities, const Json& json, ParseCitySubjsonRequest parse_city_subject_request) {
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({ city_obj["name"s].AsString(),
                          city_obj["iso_code"s].AsString(),
                          parse_city_subject_request.country_phone_code + city_obj["phone_code"s].AsString(),
                          parse_city_subject_request.country_name, parse_city_subject_request.country_iso_code,
                          parse_city_subject_request.country_time_zone,
                          parse_city_subject_request.languages });
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
            });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        ParseCitySubjson(cities, country_obj["cities"s], {
                         .country_name = country.name,
                         .country_iso_code = country.iso_code,
                         .country_phone_code = country.phone_code,
                         .country_time_zone = country.time_zone,
                         .languages = country.languages);
        }
    }
