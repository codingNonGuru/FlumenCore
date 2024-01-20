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
            friend class core::Singleton <NearbyBuffer>;

            friend class HexGrid;

            container::Array <TileType *> tiles;

            NearbyBuffer()
            {
                tiles.Initialize(4096);
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

        const Array <TileType *> &GetNearbyTiles(TileType *tile, Integer range)
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
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
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

        const container::Block <TileType *, 6> GetNearbyTiles(TileType* tile)
        {
            auto nearbyTiles = container::Block <TileType *, 6> ();

            int i = 0;
            for(Integer x = -1; x <= 1; ++x)
            {
                for(Integer y = -1; y <= 1; ++y)
                {
                    for(Integer z = -1; z <= 1; ++z)
                    {
                        if(x + y + z == 0)
                        {
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
                            if(nearbyTile != nullptr && nearbyTile != tile)
                            {
                                *nearbyTiles[i] = nearbyTile;
                                i++;
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
                            auto nearbyTile = GetTile(tile->Coordinates + Integer3(x, y, z));
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