#pragma once

#include <concepts>

#include "FlumenCore/Conventions.hpp"
#include "FlumenCore/Hex/Hex.h"
#include "FlumenCore/Singleton.h"

namespace container
{
    template <std::derived_from <core::hex::Tile> TileType>
    class HexGrid
    {
        class NearbyBuffer : public core::Singleton <NearbyBuffer>
        {
            friend class HexGrid;

            Array <core::hex::Tile *> tiles;

            NearbyBuffer()
            {
                tiles.Initialize(1024);
            }
        };

        container::Grid <TileType> tiles;

    public:
        HexGrid() {}

        HexGrid(int width, int height)
        {
            tiles.Initialize(width, height);
        }

        void Initialize(int width, int height, container::Grid <TileType>::Memory &tileMemory)
        {
            //nearbyTiles.Initialize(width * height, neighbourMemory);

            tiles.Initialize(width, height, tileMemory);
        }

        void Initialize(int width, int height)
        {
            //nearbyTiles.Initialize(width * height, neighbourMemory);

            tiles.Initialize(width, height);
        }

        TileType *Get(int x, int y)
        {
            return tiles.Get(x, y);
        }

        TileType *GetCenterTile()
        {
            return tiles.Get(tiles.GetWidth() / 2, tiles.GetHeight() / 2);
        }

        TileType *GetTile(Integer3 position)
        {
            return tiles.Get(position.x + position.z / 2, position.z);
        }

        container::Grid <TileType> &GetTiles()
        {
            return tiles;
        }

        const Array<TileType *> & GetNearbyTiles(TileType *tile, Integer range)
        {
            auto &nearbyTiles = NearbyBuffer::Get()->tiles;
            nearbyTiles.Reset();

            for(Integer x = -range; x <= range; ++x)
            {
                for(Integer y = -range; y <= range; ++y)
                {
                    for(Integer z = -range; z <= range; ++z)
                    {
                        if(x + y + z == 0)
                        {
                            auto nearbyTile = GetTile(tile->HexCoordinates + Integer3(x, y, z));
                            if(nearbyTile != nullptr)
                            {
                                *nearbyTiles.Allocate() = nearbyTile;
                            }
                        }
                    }
                }
            }

            return nearbyTiles;
        }

        const Array<TileType *> & GetTileRing(TileType *tile, Integer range)
        {
            auto &nearbyTiles = NearbyBuffer::Get()->tiles;
            nearbyTiles.Reset();

            for(Integer x = -range; x <= range; ++x)
            {
                for(Integer y = -range; y <= range; ++y)
                {
                    for(Integer z = -range; z <= range; ++z)
                    {
                        if(x + y + z == 0 && abs(x) + abs(y) + abs(z) == range * 2)
                        {
                            auto nearbyTile = GetTile(tile->HexCoordinates + Integer3(x, y, z));
                            if(nearbyTile != nullptr)
                            {
                                *nearbyTiles.Allocate() = nearbyTile;
                            }
                        }
                    }
                }
            }

            return nearbyTiles;
        }
    };
}