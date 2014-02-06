#ifndef VISION_H
#define VISION_H

#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"

//Camera constants used for distance calculation
#define Y_IMAGE_RES 480		// X Image resolution in pixels, should be 120, 240 or 480
#define VIEW_ANGLE 49		// Axis M1013
//#define VIEW_ANGLE 41.7	// Axis 206 camera
//#define VIEW_ANGLE 37.4   // Axis M1011 camera
#define PI 3.141592653		// PI

//Score limits used for target identification
#define RECTANGULARITY_LIMIT 40
#define ASPECT_RATIO_LIMIT 55

//Score limits used for hot target determination
#define TAPE_WIDTH_LIMIT 50
#define VERTICAL_SCORE_LIMIT 50
#define LR_SCORE_LIMIT 50

//Minimum area of particles to be considered
#define AREA_MINIMUM 150

//Maximum number of particles to process
#define MAX_PARTICLES 8

//Structure to represent the scores for the various tests used for target identification
struct Scores {
	double rectangularity;
	double aspectRatioVertical;
	double aspectRatioHorizontal;
};

struct TargetReport {
	int verticalIndex;
	int horizontalIndex;
	bool Hot;
	double totalScore;
	double leftScore;
	double rightScore;
	double tapeWidthScore;
	double verticalScore;
	double distance;
	int reports;
};
 
/**
 * From the 2014 Vision Sample.
 * Theoretically speaking, this should work
 * 
 * Sample program to use NIVision to find rectangles in the scene that are illuminated
 * by a LED ring light (similar to the model from FIRSTChoice). The camera sensitivity
 * is set very low so as to only show light sources and remove any distracting parts
 * of the image.
 * 
 * The CriteriaCollection is the set of criteria that is used to filter the set of
 * rectangles that are detected. In this example we're looking for rectangles with
 * a minimum width of 30 pixels and maximum of 400 pixels.
 * 
 * The algorithm first does a color threshold operation that only takes objects in the
 * scene that have a bright green color component. Then a small object filter
 * removes small particles that might be caused by green reflection scattered from other 
 * parts of the scene. Finally all particles are scored on rectangularity, and aspect ratio,
 * to determine if they are a target.
 */
class Vision {
	
public:
	static TargetReport* getReport();
	
	static TargetReport* process(ColorImage* image);
	
	/**
	 * Computes the estimated distance to a target using the height of the particle in the image. For more information and graphics
	 * showing the math behind this approach see the Vision Processing section of the ScreenStepsLive documentation.
	 * 
	 * @param image The image to use for measuring the particle estimated rectangle
	 * @param report The Particle Analysis Report for the particle
	 * @return The estimated distance to the target in feet.
	 */
	static double computeDistance (BinaryImage *image, ParticleAnalysisReport *report);

	/**
	 * Computes a score (0-100) comparing the aspect ratio to the ideal aspect ratio for the target. This method uses
	 * the equivalent rectangle sides to determine aspect ratio as it performs better as the target gets skewed by moving
	 * to the left or right. The equivalent rectangle is the rectangle with sides x and y where particle area= x*y
	 * and particle perimeter= 2x+2y
	 * 
	 * @param image The image containing the particle to score, needed to perform additional measurements
	 * @param report The Particle Analysis Report for the particle, used for the width, height, and particle number
	 * @param outer	Indicates whether the particle aspect ratio should be compared to the ratio for the inner target or the outer
	 * @return The aspect ratio score (0-100)
	 */
	static double scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical);

	/**
	 * Compares scores to defined limits and returns true if the particle appears to be a target
	 * 
	 * @param scores The structure containing the scores to compare
	 * @param vertical True if the particle should be treated as a vertical target, false to treat it as a horizontal target
	 * 
	 * @return True if the particle meets all limits, false otherwise
	 */
	static bool scoreCompare(Scores scores, bool vertical);

	/**
	 * Computes a score (0-100) estimating how rectangular the particle is by comparing the area of the particle
	 * to the area of the bounding box surrounding it. A perfect rectangle would cover the entire bounding box.
	 * 
	 * @param report The Particle Analysis Report for the particle to score
	 * @return The rectangularity score (0-100)
	 */
	static double scoreRectangularity(ParticleAnalysisReport *report);

	/**
	 * Converts a ratio with ideal value of 1 to a score. The resulting function is piecewise
	 * linear going from (0,0) to (1,100) to (2,0) and is 0 for all inputs outside the range 0-2
	 */
	static double ratioToScore(double ratio);

	/**
	 * Takes in a report on a target and compares the scores to the defined score limits to evaluate
	 * if the target is a hot target or not.
	 * 
	 * @return True if the target is hot. False if it is not.
	 */
	static bool hotOrNot(TargetReport target);
	
};

#endif
