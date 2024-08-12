import types.NetworkingPose;
import types.NetworkingTag;
import types.Translation3d;

public class ServerTest {
    private static final int PORT = 5800;

    public static void main(String[] args) {
        NetworkingServer server = new NetworkingServer(PORT);
        System.out.println("Started server at port: " + PORT);

        server.subscribe("position", (NetworkingCall<Translation3d>)(Translation3d position) -> {
            System.out.println("position: " + position.toString());
        });

        server.subscribe("note", (NetworkingCall<Translation3d>)(Translation3d note) -> {
            System.out.println("note: " + note.toString());
        });

        server.subscribe("pose", (NetworkingCall<NetworkingPose>)(NetworkingPose pose) -> {
            System.out.println("pose: " + pose.toString());
        });

        server.subscribe("tag", (NetworkingCall<NetworkingTag>)(NetworkingTag tag) -> {
            System.out.println("tag: " + tag.toString());
        });
    }
}