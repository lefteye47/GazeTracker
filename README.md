# GazeTracker
1.  Wear the glasses and open GazeTracker by executing "./opengazer"

2.  Make sure the eye image is in the middle of video and the pupil(pink circle) is well detected

3.  If not, in MainGazeTracker.cpp line 142 & 143 by changing: Eye_params.Radius_Min & Eye_params.Radius_Max

4.  In the UI, click the Calibrate button to start calibration

5.  Keep you head stable and follow the points which is shown on the screen 

6.  The initual points number are 49 you might change it in calibrator.cpp from line 149 to line 197

7.  There are 4 choices of the calibration points number which is 13,16,25 & 49

8.  If the points are changed, correspondingly, you need to change the calpoints.txt 

9.  I have modified another two files whcih are calpoints13.txt &calpoints25.txt if you need to use 13 points     
    calibration. Then just change the calpoints13.txt to calpoints.txt

10. After calibration, the blue point will be display on the screen, it is the predicted points based on your eye image

11. Press test button in the UI, it will start testing the accuracy

12. You can change the testing points number on the screen in MainGazeTracker.cpp line 263 & 264 by changing the for 
    loop

13. For the evaluation, in OutputMethod.cpp from line 47 to line 52, it is going to ouput the predict coordinate to 
    file "svm testing".
   
14. There is an "Evaluation.py" file in the project. Run the file in terminal, type "python Evaluation.py"

15. Further instructions will be in the Evaluation.py.

16. The Gaussian evaluation figure will be shown & you can consider the peak of the curve is average pixel error 
