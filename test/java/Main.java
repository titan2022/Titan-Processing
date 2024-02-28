import types.Translation3d;

public class Main {
    private static final int PORT = 5800;

    public static void main(String[] args) {
        NetworkingServer server = new NetworkingServer(PORT);
        System.out.println("Started server at port: " + PORT);
        server.subscribe("pos", (NetworkingCall<Translation3d>)(Translation3d position) -> {
            System.out.println(position.toString());
        });
    }
}