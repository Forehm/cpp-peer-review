
enum is_use_exceptions
{
    ALLOW, DENY
};

template<typename T>
struct MinMAx
{
    T min;
    T max;
};


vector<Person> LoadPersons(string_view db_name, int db_connection_timeout, is_use_exceptions db_allow_exceptions = is_use_exceptions::DENY,
    const DBLogLevel& db_log_level, const MinMAx<int>& age, string_view name_filter)
{
    DBConnector connector(db_allow_exceptions, db_log_level);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_name, db_connection_timeout);
    }
    else {
        db = connector.Connect(db_name, db_connection_timeout);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << age.min << " and "s << age.max << " "s
        << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}
