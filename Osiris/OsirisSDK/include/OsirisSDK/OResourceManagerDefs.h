#pragma once

#include <concepts>

class OResource;

template <class ResourceType>
concept OResourceType = std::derived_from<ResourceType, OResource>;

template <OResourceType ResourceType>
class OResourceManager;