#include "OObjMeshFileTokenizer.h"
#include <charconv>

void OObjMeshFileTokenizer::setLine(std::string_view aLine)
{
    m_buffer = aLine;
    m_cursor = 0;
}

bool OObjMeshFileTokenizer::hasMore() const
{
    return m_cursor < m_buffer.size();
}

std::string_view OObjMeshFileTokenizer::nextToken()
{
    m_cursor = m_buffer.find_first_not_of(" \t\r\n", m_cursor);
    if (m_cursor == std::string_view::npos) return {};

    size_t start = m_cursor;
    m_cursor = m_buffer.find_first_of(" \t\r\n", start);
    
    if (m_cursor == std::string_view::npos) {
        m_cursor = m_buffer.size();
    }
    
    return m_buffer.substr(start, m_cursor - start);
}

bool OObjMeshFileTokenizer::nextFloat(float& aValue)
{
    std::string_view token = nextToken();
    if (token.empty()) return false;

    auto result = std::from_chars(token.data(), token.data() + token.size(), aValue);
    return result.ec == std::errc() && result.ptr == token.data() + token.size();
}

bool OObjMeshFileTokenizer::nextInt(int& aValue)
{
    std::string_view token = nextToken();
    if (token.empty()) return false;

    auto result = std::from_chars(token.data(), token.data() + token.size(), aValue);
    return result.ec == std::errc() && result.ptr == token.data() + token.size();
}
