#ifndef _HS_RENDERER_OBJ_TILES_LOADER_HPP_
#define _HS_RENDERER_OBJ_TILES_LOADER_HPP_

#include "hs_renderer/obj_renderer/obj_tile.hpp"
#include "hs_renderer/obj_renderer/lod_tiles_loader.hpp"
#include "hs_renderer/obj_utility/viewing_transformer.hpp"

namespace hs
{
namespace renderer
{
namespace render
{

template <typename _Scalar, typename _Engine> class OBJTilesLoader;

template <typename _Scalar, typename _Engine>
struct LODTilesLoaderTraits<OBJTilesLoader<_Scalar, _Engine> >
{
  typedef _Scalar Scalar;
  typedef size_t LODIndex;
  typedef Scalar[6] TileIndex;
  typedef OBJTile<_Scalar> Tile;
  typedef hs::renderer::ViewingTransformer<_Scalar> ViewingTransformer;
};

template <typename _Scalar, typename _Engine>
class OBJTilesLoader :
  public LODTilesLoader<OBJTilesLoader<_Scalar, _Engine> >
{
public:
  typedef _Scalar Scalar;
  typedef _Engine Engine;
  typedef OBJTilesLoader<Scalar, Engine> Self;
  typedef LODTilesLoader<Self> Base;
  using Base::Lock;
  using Base::Unlock;
  typedef typename LODTilesLoaderTraits<Self>::LODIndex LODIndex;
  typedef typename LODTilesLoaderTraits<Self>::TileIndex TileIndex;
  typedef typename LODTilesLoaderTraits<Self>::Tile Tile;
  typedef typename LODTilesLoaderTraits<Self>::ViewingTransformer
                   ViewingTransformer;
  typedef typename Base::TileKey TileKey;
  typedef typename Base::TileContainer TileContainer;
  typedef typename Base::Goal Goal;

  //typedef EIGEN_VECTOR(Scalar, 2) Vector2;

public:
  OBJTilesLoader(const std::string& image_path)
    : engine_(image_path)
  {
  }

  Goal CalculateGoal(const ViewingTransformer& viewing_transformer,
    TileContainer& tiles, TileContainer& instant_tiles)
  {
    Goal result;
    if (!engine_.IsValid()) return result;

    Scalar viewing_unit_per_pixel =
      std::abs((viewing_transformer.right() - viewing_transformer.left()) /
      Scalar(viewing_transformer.viewport_width()));

    //²éÕÒºÏÊÊµÄLOD
     LODIndex lod_id = CalulateLod(viewing_unit_per_pixel);

     CalculateLODGoal(viewing_transformer, lod_id, result, tiles, true);


  }

  int RemoveUnnecesarryTiles(const Goal& goal, TileContainer& tiles,
    TileContainer& instant_tiles)
  {
  }

  Tile* SelectTileToLoad(const Goal& goal,
    TileContainer& tiles,
    LODIndex& lod_id, TileIndex& tile_id)
  {
    Tile* tile = NULL;

    auto itr_tile_key = goal.rbegin();
    auto itr_tile_key_end = goal.rend();
    for (; itr_tile_key != itr_tile_key_end; ++itr_tile_key)
    {
      auto itr_tile = tiles.find(*itr_tile_key);
      if (!itr_tile->second.IsLoaded())
      {
        lod_id = itr_tile_key->first;
        tile_id = itr_tile_key->second;
        tile = &(itr_tile->second);
      }
    }

    return tile;

  }

  void Load(const LODIndex& lod_id, const TileIndex& tile_id,
    Tile* tile, int* keep_loading = NULL)
  {
    engine_.LoadTile(lod_id, tile_id, *tile);
    Lock();
    tile->SetLoaded();
    Unlock();
  }

private:
  LODIndex CalulateLod(Scalar unit_per_pixel)
  {
    LODIndex lod = 0;

    if (unit_per_pixel <= 0.03125)
      lod = 21;
    else if(resolution > 0.03125 && resolution <= 0.0625)
      lod = 20;
    else if(resolution > 0.0625 && resolution <= 0.125)
      lod = 19;
    else if(resolution > 0.125 && resolution <= 0.25)
      lod = 18;
    else if(resolution > 0.25 && resolution <= 0.5)
      lod = 17;
    else if(resolution > 0.5 && resolution <= 1.0f)
      lod = 16;
    else if(resolution > 1.0f && resolution <= 2.0f)
      lod = 15;
    else if(resolution > 2.0f && resolution <= 4.0f)
      lod = 14; 
    else if(resolution > 4.0f && resolution <= 8.0f)
      lod = 13;
    else
    {
      lod = 12;
    }

    return lod;
  }

  void CalculateLODGoal(const ViewingTransformer& viewing_transformer,
    LODIndex lod_id,
    Goal& goal, TileContainer& tiles,
    bool add_to_goal)
  {

  }

private:
  Engine engine_;

}

}//namespace obj_render
}
}

#endif 
