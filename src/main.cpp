#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

//int main()
int main(int argc, char *argv[])
{
  uWS::Hub h;

  PID pid_st, pid_th;
  
  double s_Kp,s_Ki,s_Kd; // PID parameters of steering angle
  double t_Kp,t_Ki,t_Kd; // PID parameters of throttle

  switch (atoi(argv[1])) {
    case 30:
        s_Kp=0.122; s_Ki=0.000; s_Kd=0.900;
        t_Kp=0.109; t_Ki=0.000; t_Kd=-0.005;
        pid_st.Init(0.00,s_Kp,s_Ki,s_Kd);
        pid_th.Init(0.30,t_Kp,t_Ki,t_Kd);
        break;
    case 40:
        s_Kp=0.144; s_Ki=0.000; s_Kd=0.999;
        t_Kp=0.216; t_Ki=0.000; t_Kd=0.001;
        pid_st.Init(0.00,s_Kp,s_Ki,s_Kd);
        pid_th.Init(0.40,t_Kp,t_Ki,t_Kd);
        break;
    case 50:
        s_Kp=0.136; s_Ki=0.000; s_Kd=1.314;
        t_Kp=0.179; t_Ki=0.000; t_Kd=-0.009;
        pid_st.Init(0.00,s_Kp,s_Ki,s_Kd);
        pid_th.Init(0.50,t_Kp,t_Ki,t_Kd);
        break;
    case 60:
        s_Kp=0.145; s_Ki=0.000; s_Kd=2.000;
        t_Kp=0.221; t_Ki=0.000; t_Kd=-0.009;
        pid_st.Init(0.00,s_Kp,s_Ki,s_Kd);
        pid_th.Init(0.60,t_Kp,t_Ki,t_Kd);
        break;
    //case 70://
    //    s_Kp=0.154; s_Ki=0.000; s_Kd=2.279;
    //    t_Kp=0.263; t_Ki=0.000; t_Kd=-0.020;
    //    pid_st.Init(0.00,s_Kp,s_Ki,s_Kd);
    //    pid_th.Init(0.70, t_Kp,t_Ki,t_Kd);
    //    break;
    default:
        pid_st.Init("tune_steer.dat"); // steer control
        pid_th.Init("tune_throt.dat"); // throttle control
  }
  

  h.onMessage([&pid_st,&pid_th](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value, throttle_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          
          pid_st.UpdateError(cte);
          pid_th.UpdateError(cte);
          steer_value    = pid_st.TotalError();
          throttle_value = pid_th.TotalError();

          // DEBUG
          //std::cout << pid.getIC() << "-CTE: " << cte << " Steering Value: " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"]       = throttle_value;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << pid_st.getIC() << ":" << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
