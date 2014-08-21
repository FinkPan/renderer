#ifndef _HS_GRAPHICS_GRAPHICS_RENDER_LOD_TILES_LOADER_HPP_
#define _HS_GRAPHICS_GRAPHICS_RENDER_LOD_TILES_LOADER_HPP_

#include <map>
#include <thread>
#include <mutex>

#include "hs_math/linear_algebra/eigen_macro.hpp"

namespace hs
{
namespace renderer
{
namespace render
{

template <typename _Derived>
struct LODTilesLoaderTraits;

/**
 *  由TIle组成的LOD数据的读取器。
 *  使用Curiously Recurring Template Pattern实现的静态多态。
 *  其子类的Traits必须包含的成员类型有：
 *  LODIndex
 *  TileIndex
 *  Tile
 *  ViewingTransformer
 *  其子类必须包含的成员方法有：
 *  Goal CalculateGoal(const ViewingTransformer& viewing_transformer,
 *                     TileContainer& tiles, TileContainer& instant_tiles);
 *  int RemoveUnnecesarryTiles(const Goal& goal, TileContainer& tiles,
 *                             TileContainer& instant_tiles);
 *  Tile* SelectTileToLoad(const Goal& goal,
 *                         TileContainer& tiles,
 *                         LODIndex& lod_id, TileIndex& tile_id);
 *  void Load(const LODIndex& lod_id, const TileIndex& tile_id,
 *            Tile* tile, int* keep_loading);
 */
template <typename _Derived>
class LODTilesLoader
{
public:
  typedef _Derived Derived;
  typedef LODTilesLoader<Derived> Self;
  typedef typename LODTilesLoaderTraits<Derived>::LODIndex LODIndex;
  typedef typename LODTilesLoaderTraits<Derived>::TileIndex TileIndex;
  typedef typename LODTilesLoaderTraits<Derived>::Tile Tile;
  typedef typename LODTilesLoaderTraits<Derived>::ViewingTransformer
                   ViewingTransformer;
  typedef std::pair<LODIndex, TileIndex> TileKey;
  typedef EIGEN_STD_MAP(TileKey, Tile) TileContainer;
  typedef std::vector<TileKey> Goal;

public:
  Derived* derived()
  {
    return static_cast<Derived*>(this);
  }

  const Derived* derived() const
  {
    return static_cast<const Derived*>(this);
  }

  int Update(const ViewingTransformer& viewing_transformer)
  {
    Lock();
    goal_ = derived()->CalculateGoal(viewing_transformer, tiles_,
                                     instant_tiles_);
    int result = derived()->RemoveUnnecesarryTiles(goal_, tiles_,
                                                   instant_tiles_);
    Unlock();
    return result;
  }

  Goal goal()
  {
    Lock();
    Goal result = goal_;
    Unlock();
    return result;
  }

  TileContainer tiles()
  {
    Lock();
    TileContainer result = tiles_;
    Unlock();
    return result;
  }

  TileContainer instant_tiles() const
  {
    TileContainer result = instant_tiles_;
    return result;
  }

  void Start()
  {
    keep_working_ = 1;
    working_thread_ = std::thread(&Self::Work, this);
  }

  void Stop()
  {
    keep_loading_ = 0;
    keep_working_ = 0;
    if (working_thread_.joinable())
    {
      working_thread_.join();
    }
  }

protected:

  void Work()
  {
    while (keep_working_)
    {
      Lock();
      Tile* tile = NULL;
      LODIndex lod_id;
      TileIndex tile_id;
      tile = derived()->SelectTileToLoad(goal_, tiles_, lod_id, tile_id);
      Unlock();

      if (tile != NULL)
      {
        keep_loading_ = 1;
        derived()->Load(lod_id, tile_id, tile, &keep_loading_);
      }
    }
  }

protected:
  void Lock()
  {
    mutex_.lock();
  }

  void Unlock()
  {
    mutex_.unlock();
  }

protected:
  TileContainer tiles_;
  Goal goal_;
  TileContainer instant_tiles_;

  std::thread working_thread_;

  std::mutex mutex_;

  int keep_working_;
  int keep_loading_;
};

}
}
}

#endif
