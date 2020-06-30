# OpenMind
an extremely cheap and portable solution for Hand Pose Estimation suitable for educational 3d paint and other purposes using ORB-SLAM2 and a mono camera.

Author: THE Hack 2019 Dream Blue Team
Developer: @pityhero233

# notice for developers
ORBvocabulary was excluded from the repo since it exceeds the maximum file size of GitHub. Please untar it in from the Vocabulary/ folder or manually clone it from GitHub.

---

Inspiration
-----------

We first heard of the idea from Google. <https://www.tiltbrush.com/> is the *tiltbrush* project by google Inc., which use HTC VIVE VR set to do 3D painting. It is brilliant and awesome, however the cost of over 900$ made it almost impossible to be fit into daily use. Previously we have the experience of building autonomous cars, and we found several algorithms used in localization of cars can be applied to this situation, so we tried to build our own version of *TiltBrush* using cheap cameras.

What it does
------------

It is a proof-of-concept demo with two different backends, enabling user to draw art pieces in **unrestricted** space with the precision of **millimeter** level.

How I built it
--------------

Firstly,by analyzing the closed-source android client of GoPro motion camera, we hacked into a GoPro cam, by using shell scripts to control ffmpeg on Linux to convert it into UDP Streaming format we managed to turn it into a wireless webcam, and we tweaked it so that the overall delay shall not exceed 500ms. We then used CameraCalibrationToolbox from Matlab calculated its focal lengths(fx,fy) and principal points(cx,cy).

Then, by modifying the source code and recompiling source of [AprilTags Visual Fiducial System developed by University of Michigan](https://april.eecs.umich.edu/software/apriltag) together with [ORB-SLAM: A Versatile and Accurate Monocular SLAM System](http://webdiis.unizar.es/~raulmur/MurMontielTardosTRO15.pdf) , both of which are commonly used in autonomous robotics and other industrial usage, we managed to achieve millimeter-precise localization of the GoPro Camera. ![Apriltag Robots](https://res.cloudinary.com/devpost/image/fetch/s--CRq5k7W5--/c_limit,f_auto,fl_lossy,q_auto:eco,w_900/https://april.eecs.umich.edu/media/apriltag/apriltagrobots_overlay.jpg) ![Apriltag locators](https://res.cloudinary.com/devpost/image/fetch/s--DM4YKyVF--/c_limit,f_auto,fl_lossy,q_auto:eco,w_900/https://april.eecs.umich.edu/media/apriltag/tagformats_web.png) the families of Apriltag locators.

After that, we built a simple render program using Qt on C++, and we used [ZeroMQ](https://zeromq.org/) instead of boost::interprocess to achieved IPC communication.

Challenges we ran into
----------------------

Firstly, we didn't find a webcam. Geeks12 informed us just before the competition that they couldn't give us the webcam, which unfortunately was one of our core components. We called all nearby electronic stores and finally bought a GoPro Hero 7, in hope of hacking it. After nearly 5 hours of effort on cracking the camera, to our surprise we really managed to read its streaming and control it using automated commands.

Secondly, we had met great difficulty in driving the camera to work. Though OpenCV claimed to support FFmepg format, but we found out that's only the very bare-boned version of ffmpeg, which cannot decode GoPro's video compression format. We have to research a lot about the principle of ffmpeg, and (unfortunately) struggling between confusing parameters. Luckily we overcame them.

Thirdly, though some of us have experience in Computer Vision and camera calibration, we have no experience though in the Calibration based on video streaming. Dealing with that high latency required us to think of some hacks to find out which part is the latency come from. By recording a Timer and using internal timing tools, we found out that the latency comes from buffer pool of OpenCV as well as ffmpeg itself. In the end, we made it to reduce the latency and calibrate the camera.

Accomplishments that I'm proud of
---------------------------------

**Everything.**

What I learned
--------------

-   FFMPEG
-   Qt OpenGL Programming
-   QThread QTimer
-   Reverse Engineering
-   Stereo Computer Vision, Camera Intrinsic and Extrinsic, Distortion Matrix
-   How to reduce the latency of a (comparatively) complex system
-   How to divide the work properly ## What's next for Free Tilt Painter We will continue. At least I will continue.
