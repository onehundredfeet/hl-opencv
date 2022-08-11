package;

import hvector.Int2;
import hvector.Int4;
import hvector.Vec3;
import hvector.Float3;
import hl.Bytes;
import format.png.Writer;
import opencv.Native;

class Test {
	public static function main() {
		var src = Matrix.makeZeros(256, 256, CV_8UC4);
//		src.create(256, 256, CV_8UC4);
		var src_gray = new Matrix();
		var src_hsv = new Matrix();
		var thresh = 100;

		src.convertTo(src_gray, COLOR_BGR2GRAY);
		var thresholded = new Matrix();

		// Convert from BGR to HSV colorspace
		src.convertTo(src_hsv, COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		var src_threshold = new Matrix();

		var low_H = 0, low_S = 0, low_V = 0;
		final max_value_H = 360 / 2;
		final max_value = 255;
		var high_H = max_value_H;
		var high_S = max_value;
		var high_V = max_value;

		var lower = new Vec3(low_H, low_S, low_V);
		var higher = new Vec3(high_H, high_S, high_V);
		src_hsv.inRange3(src_threshold, lower, higher);

		/* 0: Binary
			1: Binary Inverted
			2: Threshold Truncated
			3: Threshold to Zero
			4: Threshold to Zero Inverted
		 */
		src_gray.threshold(thresholded, thresh, 255, THRESH_BINARY);

		var contours = thresholded.findContours(RETR_TREE, CHAIN_APPROX_SIMPLE);
        //contours
        var tmpVec = new Int2(0, 0);
        var hTemp = new Int4(0, 0, 0, 0);

        var numContours = contours.length();
        for (cidx in 0...contours.length()) {
            for (pidx in 0...contours.points(cidx)) {
                contours.getPoint( cidx, pidx, tmpVec);
                contours.getHierarchy(cidx, pidx, hTemp);
            }
        }

        trace('done number of contours ${numContours}');
	}
}
