#pragma once

#include <string_view>

class OObjMeshFileTokenizer
{
public:
    OObjMeshFileTokenizer() = default;
    
    void setLine(std::string_view aLine);

    bool hasMore() const;
    std::string_view nextToken();
    bool nextFloat(float& aValue);
    bool nextInt(int& aValue);

private:
    std::string_view m_buffer;
    size_t m_cursor{ 0 };
};

