/**
* This file is part of ORB-SLAM2.
*
* Lennart Haller, 2018
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DENSE_MAP_H_
#define DENSE_MAP_H_

#include "KeyFrame.h"
#include "Thirdparty/mt_task_queue/include/TaskQueue.h"

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

#include <mutex>
#include <thread>
#include <map>
#include <functional>

namespace ORB_SLAM2
{

  class DenseMap
{
  typedef pcl::PointXYZRGBA PointRGBD;
  typedef pcl::PointCloud<DenseMap::PointRGBD> PointCloudRGBD;

  public:
    DenseMap ();
    ~DenseMap ();
    void AddFrame (const unsigned long int frame_id, const cv::Mat &position, const cv::Mat &rgb_img, const cv::Mat &depth_img);
    bool IsNew () {return is_new_;}
    void GetMap (cv::Mat &rgb_map, cv::Mat &depth_map);
    void UpdateFramePosition (unsigned long int frame_id, cv::Mat &position);

  private:
      struct MapImageEntry {
        MapImageEntry () {}
        MapImageEntry (cv::Mat rgb, cv::Mat depth, cv::Mat pos)
        : rgb_img(rgb), depth_img (depth), position (pos) {}

        cv::Mat rgb_img;
        cv::Mat depth_img;
        cv::Mat position;
      };

    void AddFrameToMap (unsigned long int frame_id);
    void MatsToPclRGBDCloud (cv::Mat depth_mat, cv::Mat rgb_mat, DenseMap::PointCloudRGBD::Ptr cloud);
    void PclRGBDCloudToMats (DenseMap::PointCloudRGBD::Ptr cloud, cv::Mat depth_mat, cv::Mat rgb_mat);
    DenseMap::PointCloudRGBD::Ptr FitFrame (DenseMap::PointCloudRGBD::Ptr map, DenseMap::PointCloudRGBD::Ptr frame, cv::Mat);

    bool is_new_;
    bool create_dense_map_;
    unsigned int current_task_id_;
    std::map <unsigned long int, MapImageEntry> raw_map_data_;
    cv::Mat rgb_map_;
    cv::Mat depth_map_;
    TaskQueue::TaskQueue<DenseMap::PointCloudRGBD::Ptr, DenseMap::PointCloudRGBD::Ptr, DenseMap::PointCloudRGBD::Ptr, cv::Mat> *task_queue_;
    std::mutex map_mutex_;
    std::mutex raw_data_mutex_;


};

} // namespace ORB_SLAM

#endif // DENSE_MAP_H_
