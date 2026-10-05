#include "JsonUtility.h"

bool JsonUtility::IsContain(const nlohmann::json& json,const std::string& key)
{
	return json.contains(key);
}

FloatVector2 JsonUtility::GetPosToFloat2D(const nlohmann::json& json)
{
    FloatVector2 ret{ 0, 0 };

    if (json.contains("Position"))
    {
        const auto& pos = json["Position"];

        if (pos.contains("x") && pos.contains("y"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
        }
    }
    else if (json.contains("Pos"))
    {
        const auto& pos = json["Pos"];

        if (pos.contains("x") && pos.contains("y"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
        }
    }
    else if (json.is_array() && json.size() >= 2)
    {
        ret.x = json[0];
        ret.y = json[1];
    }
    else if (json.contains("x") && json.contains("y"))
    {
        ret.x = json["x"];
        ret.y = json["y"];
    }

    return ret;
}

FloatVector2 JsonUtility::GetPosToInt2D(const nlohmann::json& json)
{
    IntVector2 ret{ 0, 0 };

    if (json.contains("Position"))
    {
        const auto& pos = json["Position"];

        if (pos.contains("x") && pos.contains("y"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
        }
    }
    else if (json.contains("Pos"))
    {
        const auto& pos = json["Pos"];

        if (pos.contains("x") && pos.contains("y"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
        }
    }
    else if (json.is_array() && json.size() >= 2)
    {
        ret.x = json[0];
        ret.y = json[1];
    }
    else if (json.contains("x") && json.contains("y"))
    {
        ret.x = json["x"];
        ret.y = json["y"];
    }

    return ret;
}

VECTOR JsonUtility::GetPosToFloat3D(const nlohmann::json& json)
{
    VECTOR ret{ 0, 0,0 };

    if (json.contains("Position"))
    {
        const auto& pos = json["Position"];

        if (pos.contains("x") && pos.contains("y") && pos.contains("z"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
            ret.z = pos["z"];
        }
    }
    else if (json.contains("Pos"))
    {
        const auto& pos = json["Pos"];

        if (pos.contains("x") && pos.contains("y")&& pos.contains("z"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
            ret.z = pos["z"];
        }
    }
    else if (json.is_array() && json.size() >= 3)
    {
        ret.x = json[0];
        ret.y = json[1];
        ret.z = json[2];
    }
    else if (json.contains("x") && json.contains("y") && json.contains("z"))
    {
        ret.x = json["x"];
        ret.y = json["y"];
        ret.z = json["z"];
    }
    return ret;
}

IntVector3 JsonUtility::GetPosToInt3D(const nlohmann::json& json)
{
    IntVector3 ret = IntVector3( 0, 0,0 );

    if (json.contains("Position"))
    {
        const auto& pos = json["Position"];

        if (pos.contains("x") && pos.contains("y") && pos.contains("z"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
            ret.z = pos["z"];
        }
    }
    else if (json.contains("Pos"))
    {
        const auto& pos = json["Pos"];

        if (pos.contains("x") && pos.contains("y") && pos.contains("z"))
        {
            ret.x = pos["x"];
            ret.y = pos["y"];
            ret.z = pos["z"];
        }
    }
    else if (json.is_array() && json.size() >= 3)
    {
        ret.x = json[0];
        ret.y = json[1];
        ret.z = json[2];
    }
    else if (json.contains("x") && json.contains("y") && json.contains("z"))
    {
        ret.x = json["x"];
        ret.y = json["y"];
        ret.z = json["z"];
    }
    return ret;
}

VECTOR JsonUtility::GetColorFloat(const nlohmann::json& json)
{
    VECTOR ret;
    if (json.contains("Color"))
    {
        const auto& color = json["Color"];

        if (color.contains("r") && color.contains("g") && color.contains("b"))
        {
            ret.x = color["r"];
            ret.y = color["g"];
            ret.z = color["b"];
        }
    }
    else if (json.is_array() && json.size() >= 3)
    {
        ret.x = json[0];
        ret.y = json[1];
        ret.z = json[2];
    }
    else if (json.contains("r") && json.contains("g") && json.contains("b"))
    {
        ret.x = json["r"];
        ret.y = json["g"];
        ret.z = json["b"];
    }

    return ret;
}
