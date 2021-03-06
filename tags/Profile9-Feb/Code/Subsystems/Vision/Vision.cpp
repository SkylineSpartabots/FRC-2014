#include "Vision.h"

Scores *scores;
TargetReport target;
int verticalTargets[MAX_PARTICLES];
int horizontalTargets[MAX_PARTICLES];
int verticalTargetCount, horizontalTargetCount;
Threshold threshold( //HSV threshold criteria, ranges are in that order ie. Hue is 60-100
		100,  150,  // original values are 105 and 137  - max = 360
		220,  255,  // original values are 230 and 255  - max = 255
		50, 200);   // original values are 133 and 183  - max = 255
// 52 - 71
ParticleFilterCriteria2 criteria[] = { //Particle filter criteria, used to filter out small particles
		{IMAQ_MT_AREA, AREA_MINIMUM, 65535, false, false}
};

TargetReport* Vision::getReport() {
	return &target;
}

TargetReport* Vision::process(ColorImage* image) {
	//SmartDashboard::PutString("Trace", "TresholdHSV");
	BinaryImage *thresholdImage = image->ThresholdHSV(threshold);	// get just the green target pixels
	//thresholdImage->Write("/threshold.bmp");
	//BinaryImage *filteredImage = thresholdImage;
	////SmartDashboard::PutString("Trace", "Particle Filter");
	BinaryImage *filteredImage = thresholdImage->ParticleFilter(criteria, 1);	//Remove small particles
	//filteredImage->Write("Filtered.bmp");
	
	//SmartDashboard::PutString("Trace", "Get Ordered Particle Analysis Reports");
	vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  //get a particle analysis report for each particle
	target.distance = -1;
	target.reports = reports->size();
	
	verticalTargetCount = horizontalTargetCount = 0;
	//Iterate through each particle, scoring it and determining whether it is a target or not
	if(reports->size() > 0)
	{
		scores = new Scores[reports->size()];
		for (unsigned int i = 0; i < MAX_PARTICLES && i < reports->size(); i++) {
			//SmartDashboard::PutString("Trace", "Score particles");
			ParticleAnalysisReport *report = &(reports->at(i));
			
			//Score each particle on rectangularity and aspect ratio
			scores[i].rectangularity = Vision::scoreRectangularity(report);
			scores[i].aspectRatioVertical = Vision::scoreAspectRatio(filteredImage, report, true);
			scores[i].aspectRatioHorizontal = Vision::scoreAspectRatio(filteredImage, report, false);			
			
			//SmartDashboard::PutString("Trace", "Check particle if horiz or vert target");
			//Check if the particle is a horizontal target, if not, check if it's a vertical target
			if(Vision::scoreCompare(scores[i], false))
			{
				printf("particle: %d  is a Horizontal Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
				horizontalTargets[horizontalTargetCount++] = i; //Add particle to target array and increment count
			} else if (Vision::scoreCompare(scores[i], true)) {
				printf("particle: %d  is a Vertical Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
				verticalTargets[verticalTargetCount++] = i;  //Add particle to target array and increment count
			} else {
				printf("particle: %d  is not a Target centerX: %d  centerY: %d \n", i, report->center_mass_x, report->center_mass_y);
			}
			printf("Scores rect: %f  ARvert: %f \n", scores[i].rectangularity, scores[i].aspectRatioVertical);
			printf("ARhoriz: %f  \n", scores[i].aspectRatioHorizontal);	
		}

		//Zero out scores and set verticalIndex to first target in case there are no horizontal targets
		target.totalScore = target.leftScore = target.rightScore = target.tapeWidthScore = target.verticalScore = 0;
		target.verticalIndex = verticalTargets[0];
		target.distance = -1;
		for (int i = 0; i < verticalTargetCount; i++)
		{
			ParticleAnalysisReport *verticalReport = &(reports->at(verticalTargets[i]));
			for (int j = 0; j < horizontalTargetCount; j++)
			{
				ParticleAnalysisReport *horizontalReport = &(reports->at(horizontalTargets[j]));
				double horizWidth, horizHeight, vertWidth, leftScore, rightScore, tapeWidthScore, verticalScore, total;
				//SmartDashboard::PutString("Trace", "Horiz report imaqMeasureParticle");
				//Measure equivalent rectangle sides for use in score calculation
				imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &horizWidth);
				imaqMeasureParticle(filteredImage->GetImaqImage(), verticalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &vertWidth);
				imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &horizHeight);
				
				//SmartDashboard::PutString("Trace", "Horiz report ratioToScore");
				//Determine if the horizontal target is in the expected location to the left of the vertical target
				leftScore = Vision::ratioToScore(1.2*(verticalReport->boundingRect.left - horizontalReport->center_mass_x)/horizWidth);
				//Determine if the horizontal target is in the expected location to the right of the  vertical target
				rightScore = Vision::ratioToScore(1.2*(horizontalReport->center_mass_x - verticalReport->boundingRect.left - verticalReport->boundingRect.width)/horizWidth);
				//Determine if the width of the tape on the two targets appears to be the same
				tapeWidthScore = Vision::ratioToScore(vertWidth/horizHeight);
				//Determine if the vertical location of the horizontal target appears to be correct
				verticalScore = Vision::ratioToScore(1-(verticalReport->boundingRect.top - horizontalReport->center_mass_y)/(4*horizHeight));
				total = leftScore > rightScore ? leftScore:rightScore;
				total += tapeWidthScore + verticalScore;
				
				//If the target is the best detected so far store the information about it
				if(total > target.totalScore)
				{
					target.horizontalIndex = horizontalTargets[j];
					target.verticalIndex = verticalTargets[i];
					target.totalScore = total;
					target.leftScore = leftScore;
					target.rightScore = rightScore;
					target.tapeWidthScore = tapeWidthScore;
					target.verticalScore = verticalScore;
				}
			}
			//SmartDashboard::PutString("Trace", "Check hot");
			//Determine if the best target is a Hot target
			target.Hot = Vision::hotOrNot(target);
		}
		
		if(verticalTargetCount > 0)
		{
			//SmartDashboard::PutString("Trace", "Check distance");
			//Information about the target is contained in the "target" structure
			//To get measurement information such as sizes or locations use the
			//horizontal or vertical index to get the particle report as shown below
			ParticleAnalysisReport *distanceReport = &(reports->at(target.verticalIndex));
			double distance = Vision::computeDistance(filteredImage, distanceReport);
			if(target.Hot)
			{
				target.distance = distance;
				printf("Hot target located \n");
				printf("Distance: %f \n", distance);
			} else {
				target.distance = distance;
				printf("No hot target present \n");
				printf("Distance: %f \n", distance);
			}
		}
	}

	// be sure to delete images after using them
	delete filteredImage;
	delete thresholdImage;
	delete image;
	
	//delete allocated reports and Scores objects also
	delete scores;
	delete reports;
	
	return &target;
}

double Vision::computeDistance(BinaryImage *image, ParticleAnalysisReport *report) {
	double rectLong, height;
	int targetHeight;
	
	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
	//using the smaller of the estimated rectangle long side and the bounding rectangle height results in better performance
	//on skewed rectangles
	height = min(report->boundingRect.height, rectLong);
	targetHeight = 32;
	
	return Y_IMAGE_RES * targetHeight / (height * 12 * 2 * tan(VIEW_ANGLE*PI/(180*2)));
}

double Vision::scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical){
	double rectLong, rectShort, idealAspectRatio, aspectRatio;
	idealAspectRatio = vertical ? (4.0/32) : (23.5/4);	//Vertical reflector 4" wide x 32" tall, horizontal 23.5" wide x 4" tall
	
	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);
	
	//Divide width by height to measure aspect ratio
	if(report->boundingRect.width > report->boundingRect.height){
		//particle is wider than it is tall, divide long by short
		aspectRatio = ratioToScore(((rectLong/rectShort)/idealAspectRatio));
	} else {
		//particle is taller than it is wide, divide short by long
		aspectRatio = ratioToScore(((rectShort/rectLong)/idealAspectRatio));
	}
	return aspectRatio;		//force to be in range 0-100
}

bool Vision::scoreCompare(Scores scores, bool vertical){
	bool isTarget = true;

	isTarget &= scores.rectangularity > RECTANGULARITY_LIMIT;
	if(vertical){
		isTarget &= scores.aspectRatioVertical > ASPECT_RATIO_LIMIT;
	} else {
		isTarget &= scores.aspectRatioHorizontal > ASPECT_RATIO_LIMIT;
	}

	return isTarget;
}

double Vision::scoreRectangularity(ParticleAnalysisReport *report){
	if(report->boundingRect.width*report->boundingRect.height !=0){
		return 100*report->particleArea/(report->boundingRect.width*report->boundingRect.height);
	} else {
		return 0;
	}	
}	

double Vision::ratioToScore(double ratio)
{
	return (max(0, min(100*(1-fabs(1-ratio)), 100)));
}

bool Vision::hotOrNot(TargetReport target)
{
	bool isHot = true;
	
	isHot &= target.tapeWidthScore >= TAPE_WIDTH_LIMIT;
	isHot &= target.verticalScore >= VERTICAL_SCORE_LIMIT;
	isHot &= (target.leftScore > LR_SCORE_LIMIT) | (target.rightScore > LR_SCORE_LIMIT);
	
	return isHot;
}
