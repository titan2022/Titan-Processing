# Examples
## Networking
### No reply
Sends a packet to specified address with one vector
### With reply
Same as no reply example but includes a return vector
## April Tag
### Local Detection
Detects specified AprilTag and sends to specified dashboard address
### Global Detection
Detects all AprilTags, maps to global position according to `apriltags.json` and sends to specified dashboard address
## Helper
### Read Config
Reads `processing.cfg` file and outputs port
### Undistort
Shows undistorted camera stream based on config settings