/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TANGO_POINT_CLOUD_POINT_CLOUD_APP_H_
#define TANGO_POINT_CLOUD_POINT_CLOUD_APP_H_

#include <jni.h>
#include <memory>
#include <string>

#include <tango_client_api.h>  // NOLINT
#include <tango-gl/util.h>
#include <tango_support_api.h>

#include <tango-point-cloud/scene.h>

namespace tango_point_cloud {

// PointCloudApp handles the application lifecycle and resources.
class PointCloudApp {
 public:
  // Constructor and deconstructor.
  PointCloudApp();
  ~PointCloudApp();

  // Check that the installed version of the Tango API is up to date.
  //
  // @return returns true if the application version is compatible with the
  //         Tango Core version.
  bool CheckTangoVersion(JNIEnv* env, jobject caller_activity,
                         int min_tango_version);

  // Called when Tango Service is connected successfully.
  bool OnTangoServiceConnected(JNIEnv* env, jobject binder);

  // Setup the configuration file for the Tango Service.
  int TangoSetupConfig();

  // Connect the onPoseAvailable callback.
  int TangoConnectCallbacks();

  // Connect to Tango Service.
  // This function will start the Tango Service pipeline, in this case, it will
  // start Motion Tracking and Depth Sensing callbacks.
  bool TangoConnect();

  // Disconnect from Tango Service, release all the resources that the app is
  // holding from the Tango Service.
  void TangoDisconnect();

  // Explicitly reset motion tracking and restart the pipeline.
  // Note that this will cause motion tracking to re-initialize.
  void TangoResetMotionTracking();

  // Tango Service point cloud callback function for depth data. Called when new
  // new point cloud data is available from the Tango Service.
  //
  // @param pose: The current point cloud returned by the service,
  //              caller allocated.
  void onPointCloudAvailable(const TangoXYZij* xyz_ij);

  // Allocate OpenGL resources for rendering, mainly for initializing the Scene.
  void InitializeGLContent();

  // Setup the view port width and height.
  void SetViewPort(int width, int height);

  // Main render loop.
  void Render();

  // Release all non-OpenGL allocated resources.
  void DeleteResources();

  // Return total point count in the current depth frame.
  int GetPointCloudVerticesCount();

  // Return the average depth of points in the current depth frame.
  float GetAverageZ();
  // Set render camera's viewing angle, first person, third person or top down.
  //
  // @param: camera_type, camera type includes first person, third person and
  //         top down
  void SetCameraType(tango_gl::GestureCamera::CameraType camera_type);

  // Touch event passed from android activity. This function only supports two
  // touches.
  //
  // @param: touch_count, total count for touches.
  // @param: event, touch event of current touch.
  // @param: x0, normalized touch location for touch 0 on x axis.
  // @param: y0, normalized touch location for touch 0 on y axis.
  // @param: x1, normalized touch location for touch 1 on x axis.
  // @param: y1, normalized touch location for touch 1 on y axis.
  void OnTouchEvent(int touch_count, tango_gl::GestureCamera::TouchEvent event,
                    float x0, float y0, float x1, float y1);

  // Set screen rotation index.
  //
  // @param screen_roatation: the screen rotation index,
  //    the index is following Android screen rotation enum.
  //    see Android documentation for detail:
  //    http://developer.android.com/reference/android/view/Surface.html#ROTATION_0
  void SetScreenRotation(int rotation_index);

 private:
  // Update the current point data.
  void UpdateCurrentPointData();

  // Point data manager.
  TangoSupportPointCloudManager* point_cloud_manager_;
  TangoXYZij* front_cloud_;
  float point_cloud_average_depth_;

  // main_scene_ includes all drawable object for visualizing Tango device's
  // movement and point cloud.
  Scene main_scene_;

  // Tango configration file, this object is for configuring Tango Service setup
  // before connect to service. For example, we turn on the depth sensing in
  // this example.
  TangoConfig tango_config_;

  // Last valid transforms.
  glm::mat4 start_service_T_device_;
  glm::mat4 start_service_opengl_T_depth_tango_;

  // Screen rotation index.
  int screen_rotation_;
};
}  // namespace tango_point_cloud

#endif  // TANGO_POINT_CLOUD_POINT_CLOUD_APP_H_
