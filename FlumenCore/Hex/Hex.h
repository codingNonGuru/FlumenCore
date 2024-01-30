#pragma once

#include "FlumenCore/Conventions.hpp"

namespace core::hex
{
    struct Tile
    {
        Integer3 Coordinates;

        Tile() {}

        Tile(Integer3 coordinates) : Coordinates(coordinates) {}

        Integer GetDistanceTo(const Tile& other) 
        {
            auto direction = this->Coordinates - other.Coordinates;
            return (abs(direction.x) + abs(direction.y) + abs(direction.z)) / 2;
        }

        Integer2 GetSquarePosition()
        {
            return {Coordinates.x + Coordinates.z / 2, Coordinates.z};
        }
    };

    class PhysicalTile : public Tile
    {
        Position2 Position;

        Float GetPhysicalDistanceTo(const PhysicalTile& other)
        {
            auto direction = this->Position - other.Position;
            return glm::length(direction);
        }
    };

    template <std::derived_from <core::hex::Tile> TileType>
    struct HexEdge
    {
        TileType *First;

        TileType *Second;
    };
}