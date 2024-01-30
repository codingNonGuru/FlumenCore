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
    protected:
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

        container::Block <TileType *, 6> GetNearbyTiles(TileType* tile)
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

    template <
        std::derived_from <core::hex::Tile> TileType, 
        std::derived_from <core::hex::HexEdge <TileType>> EdgeType
        >
    class EdgyHexGrid : public HexGrid <TileType>
    {
        container::Grid <EdgeType> edges;

    public:
        void AddEdges()
        {
            edges.Initialize(this->tiles.GetWidth() * 3, this->tiles.GetHeight());
        }

        Integer2 GetEdgeCoordinates(TileType *first, TileType *second)
        {
            auto firstPosition = first->GetSquarePosition();
            auto secondPosition = second->GetSquarePosition();

            if(firstPosition.y == secondPosition.y)
            {
                auto x = firstPosition.x < secondPosition.x ? firstPosition.x : secondPosition.x;
                auto y = firstPosition.y;

                return {x, y};
            }
            else
            {
                int y = firstPosition.y < secondPosition.y ? firstPosition.y : secondPosition.y;
                auto x = this->tiles.GetWidth() + firstPosition.x + secondPosition.x + 1;

                return {x, y};
            }
        }

        EdgeType *GetEdge(TileType *first, TileType *second)
        {
            auto firstPosition = first->GetSquarePosition();
            auto secondPosition = second->GetSquarePosition();

            if(firstPosition.y == secondPosition.y)
            {
                auto x = firstPosition.x < secondPosition.x ? firstPosition.x : secondPosition.x;
                auto y = firstPosition.y;

                /*if(abs(firstPosition.x - secondPosition.x) > 1)
                {
                    x = this->tiles.GetWidth() - 1;
                }*/

                if(y % 2 == 0)
                {
                    //x += 1;
                }

                //std::cout << x << " " << y << "\n";

                return edges.Get(x, y);
            }
            else
            {
                int y = firstPosition.y < secondPosition.y ? firstPosition.y : secondPosition.y;
            
                /*if(firstPosition.y % 2 == 0)
                {
                    y = firstPosition.y / 2;
                }
                else
                {
                    y = secondPosition.y / 2;
                }*/

                auto x = this->tiles.GetWidth() + firstPosition.x + secondPosition.x + 1;
                /*if(abs(firstPosition.x - secondPosition.x) > 3)
                {
                    x = this->tiles.GetWidth();
                }*/
                /*else if(abs(firstPosition.y - secondPosition.y) > 1)
                {
                    y = this->tiles.GetHeight() - 1;
                }*/

                if(y % 2 == 0)
                {
                    //x += 1;
                }

                //std::cout << x << " " << y << "\n";

                return edges.Get(x, y);
            }
        }

        container::Grid <EdgeType> &GetEdges() {return edges;}
    };
}