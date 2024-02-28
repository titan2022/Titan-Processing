package types;

public class NetworkingVector {
    public String objectName;
    public Translation3d vector;

    public NetworkingVector(String objectName, Translation3d vector) {
        this.objectName = objectName;
        this.vector = vector;
    }
}