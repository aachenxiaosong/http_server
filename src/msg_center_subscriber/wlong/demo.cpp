#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../CJsonObject.hpp"

using namespace std;
using namespace neb;

CJsonObject gjson;

int _parse_home(CJsonObject& djson, CJsonObject& sjson) {
  int ivalue;
  string svalue;
  CJsonObject jvalue;
  if (true != sjson.Get("id", ivalue)) {
    goto L_ERROR;
  }
  djson.Add("id", ivalue);
  if (true != sjson.Get("extension", jvalue)) {
    goto L_ERROR;
  }
  if (true != sjson["extension"].Get("homeNo", svalue)) {
    goto L_ERROR;
  }
  djson.Add("number", svalue);
  if (true != sjson["extension"].Get("floorLocation", svalue)) {
    goto L_ERROR;
  }
  djson.Add("floor", svalue);
  if (true != sjson["extension"].Get("clusterControllerId", svalue)) {
    goto L_ERROR;
  }
  djson.Add("liftHall", svalue);
  return 0;
L_ERROR:
  cout << "parse home failed" << endl;
  return -1;
}

int _parse_unit(CJsonObject& djson, CJsonObject& sjson) {
  int ivalue;
  string svalue;
  CJsonObject jvalue;
  if (true != sjson.Get("id", ivalue)) {
    goto L_ERROR;
  }
  djson.Add("id", ivalue);
  if (true != sjson.Get("extension", jvalue)) {
    goto L_ERROR;
  }
  if (true != sjson["extension"].Get("unitNo", svalue)) {
    goto L_ERROR;
  }
  djson.Add("number", svalue);
  if (true != sjson["extension"].Get("floorMap", svalue)) {
    goto L_ERROR;
  }
  djson.Add("floorMap", CJsonObject(svalue));
  if (true != sjson.Get("spaceEntityList", jvalue) ||
      false == jvalue.IsArray() ||
      0 == jvalue.GetArraySize()) {
    goto L_ERROR;
  }
  djson.AddEmptySubArray("homes");
  for (int i = 0; i < jvalue.GetArraySize(); i++) {
    djson["homes"].Add(CJsonObject("{}"));
    if (0 != _parse_home(djson["homes"][i], jvalue[i])) {
      goto L_ERROR;
    }
  }
  return 0;
L_ERROR:
  cout << "parse unit failed" << endl;
  return -1;
}

int _parse_building(CJsonObject& djson, CJsonObject& sjson) {
  int ivalue;
  string svalue;
  CJsonObject jvalue;
  if (true != sjson.Get("id", ivalue)) {
    goto L_ERROR;
  }
  djson.Add("id", ivalue);
  if (true != sjson.Get("extension", jvalue) ||
      true != sjson["extension"].Get("buildingNo", svalue)) {
    goto L_ERROR;
  }
  djson.Add("number", svalue);
  if (true != sjson.Get("spaceEntityList", jvalue) ||
      false == jvalue.IsArray() ||
      0 == jvalue.GetArraySize()) {
    goto L_ERROR;
  }
  djson.AddEmptySubArray("units");
  for (int i = 0; i < jvalue.GetArraySize(); i++) {
    djson["units"].Add(CJsonObject("{}"));
    if (0 != _parse_unit(djson["units"][i], jvalue[i])) {
      goto L_ERROR;
    }
  }
  return 0;
L_ERROR:
  cout << "parse building failed" << endl;
  return -1;
}

int _parse(CJsonObject& djson, CJsonObject& sjson) {
  string svalue;
  CJsonObject jvalue;
  if (true != sjson.Get("reqId", svalue)) {
    goto L_ERROR;
  }
  djson.Add("reqId", svalue);

  if (true != sjson.Get("entityCode", svalue)) {
    goto L_ERROR;
  }
  djson.Add("entityCode", svalue);

  if (true != sjson.Get("extendData", jvalue)) {
    goto L_ERROR;
  }
  djson.Add("wlong", jvalue);

  if (true != sjson.Get("spaceFramework", jvalue) ||
      true != sjson["spaceFramework"].Get("spaceEntityList", jvalue) ||
      false == jvalue.IsArray() ||
      0 == jvalue.GetArraySize()) {
    goto L_ERROR;
  }
  djson.AddEmptySubArray("buildings");
  for (int i = 0; i < jvalue.GetArraySize(); i++) {
    djson["buildings"].Add(CJsonObject("{}"));
    if (0 != _parse_building(djson["buildings"][i], jvalue[i])) {
      goto L_ERROR;
    }
  }
  return 0;
L_ERROR:
  cout << "parse failed" << endl;
  return -1;
}

int main()
{
  ifstream fin("json.txt", ios::in);
  if (!fin) {
    cout << "read json.txt failed" << endl;
    return -1;
  }
  stringstream sstream;
  sstream << fin.rdbuf();
  string str(sstream.str());
  CJsonObject sjson(str);
  _parse(gjson, sjson);
  cout << gjson.ToFormattedString() << endl;
  fin.close();
  return 0;
}

