package types;

public class NetworkingPose {
    public String objectName;
    public Translation3d position;
    public Translation3d rotation;

    public NetworkingPose(String objectName, Translation3d position, Translation3d rotation) {
        this.objectName = objectName;
        this.position = position;
        this.rotation = rotation;
    }
}