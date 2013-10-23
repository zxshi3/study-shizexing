/**
 * This is a example code to parse nmea rmc format gps fix
 *
 * \ref http://aprs.gids.nl/nmea/#rmc
 * eg4. $GPRMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,ddmmyy,x.x,a*hh
 * 1    = UTC of position fix
 * 2    = Data status (V=navigation receiver warning) (A=OK)
 * 3    = Latitude of fix
 * 4    = N or S
 * 5    = Longitude of fix
 * 6    = E or W
 * 7    = Speed over ground in knots
 * 8    = Track made good in degrees True
 * 9    = UTC date
 * 10   = Magnetic variation degrees (Easterly var. subtracts from true course)
 * 11   = E or W
 * 12   = Checksum
 *
 * NOTE: in ublox example, it has one more ',' after UTC Date. 
 *       and it does not follow above format, a*hh, a is not E or W
 * eg:
 * "$GPRMC,201018.00,A,3723.14539,N,12200.26880,W,0.008,,211013,,,D*6F"
 * 1    = 201018.00
 * 2    = A
 * 3    = 3723.14539
 * 4    = N
 * 5    = 12200.26880
 * 6    = W
 * 7    = 0.008
 * 8    = nil
 * 9    = 211013
 * 10   = nil
 * 11   = nil
 * 12   = D*7F
 */
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <ssteam>
#include <string>
#include <ctime>

struct GpsFix {
    time_t t;   // TODO, change to boost::chrono 
    bool valid;
    double lat; // N +, S -
    double lon; // E +, W -
    double speed;
    double track;   // ?
    double var;     // Magnetic variation degrees
    std::string toString() {
        std::ostringstream ss;
        ss << "GPS fix:\n";
        ss << "\t valid : " << (valid ? "true" : "false") << "\n";
        ss << "\t time : " << ctime(&t) << "\n";
        ss << "\t latitude : " << lat << "\n";
        ss << "\t longitude : " << lon << "\n";
        ss << "\t speed : " << speed << "\n";
        ss << "\t track : " << track << "\n";
        ss << "\t magnetic variation : " << var << "\n";
        return ss.str();
    }
};

static const std::string rmc = "$GPRMC,201018.00,A,3723.14539,N,12200.26880,W,0.008,,211013,,,D*6F";

struct GpsFix parseRmc(const std::string & rmc) ;

int main(int argc, char** argv) {
    struct GpsFix fix = parseRmc(rmc);
    std::cout << fix.toString() << std::endl;
    return 0;
}

struct GpsFix parseRmc(const std::string & rmc) {
    struct GpsFix fix = {0};
    fix.valid = false;
    size_t idx = rmc.find(',');
    if (idx == std::string::npos) {
        return fix;
    }
    std::string type = rmc.substr(0, idx);
    do {
        if (type == "$GPRMC") {
            struct tm cur = {0};
            // 1, time
            size_t idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string time = rmc.substr(idx + 1, idx2 - idx - 1);
            double dTime = boost::lexical_cast<double, std::string>(time);
            cur.tm_hour = dTime / 10000;
            cur.tm_min = (static_cast<int>(dTime) % 10000) / 100;
            cur.tm_sec = static_cast<int>(dTime) % 100;
            int ms = (dTime - (static_cast<int>(dTime))) * 100;
            idx = idx2;
            // 2, status
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string status = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            if (status == "V") {
                // invalid
                continue;
            }
            // 3, latitude
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string lat = rmc.substr(idx + 1, idx2 - idx - 1);
            double dLat = boost::lexical_cast<double, std::string>(lat) / 100;
            idx = idx2;
            // 4, N or S
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string lat2 = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            if (lat2 == "N") {
                fix.lat = dLat;
            } else {
                fix.lat = -dLat;
            }
            // 5, longitude
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string lon = rmc.substr(idx + 1, idx2 - idx - 1);
            double dLon = boost::lexical_cast<double, std::string>(lon) / 100;
            idx = idx2;
            // 6, E or W
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string lon2 = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            if (lon2 == "E") {
                fix.lon = dLon;
            } else {
                fix.lon = -dLon;
            }
            // 7, Speed over ground in knots
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string speed = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            double dSpeed = boost::lexical_cast<double, std::string>(speed);
            fix.speed = dSpeed;
            // 8, Track made good in degrees True ?
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string track = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            if (track != "") {
                fix.track = boost::lexical_cast<double, std::string>(track);
            }
            // 9, UTC date
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string date = rmc.substr(idx + 1, idx2 - idx - 1);
            int dDate = boost::lexical_cast<int, std::string>(date);
            cur.tm_mday = dDate / 10000;
            cur.tm_mon = (dDate % 10000) / 100 - 1;
            cur.tm_year = (dDate % 100) + 100;
            idx = idx2;
            // 10, Magnetic variation degrees
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string var = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            fix.var = boost::lexical_cast<double, std::string>(var);
            fix.t = mktime(&cur);
            fix.valid = true;
            // 11, E or W
            idx2 = rmc.find(',', idx + 1);
            if (idx2 == std::string::npos) {
                continue;
            }
            std::string lon22 = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            // 12, Checksum
            idx2 = rmc.find(',', idx + 1);
            std::string checksum = rmc.substr(idx + 1, idx2 - idx - 1);
            idx = idx2;
            //std::cout << "curtime : " << asctime(&cur) << std::endl;
            time_t t = mktime(&cur);
            std::cout << "curtime : " << ctime(&t) << std::endl;
        }
    } while(0);
    return fix;
}