package types;

public class NetworkingTag {
    public String objectName;
    public Translation3d position;
    public Translation3d rotation;
    public int id;

    public NetworkingTag(String objectName, Translation3d position, Translation3d rotation, int id) {
        this.objectName = objectName;
        this.position = position;
        this.rotation = rotation;
        this.id = id;
    }

    public String toString() {
        return id + ": " + position.toString() + " " + rotation.toString();
    }
}