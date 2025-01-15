import types.NetworkingPose;
import types.NetworkingTag;
import types.Translation3d;

public class ServerTest {
    private static final double SUCCESS_RATIO = 0.5; // Ratio of correct packets needed to pass the test

    private static final int PORT = 5801;
    private static final int PACKETS_SENT = 10; // Per type
    private static final double FLOAT_ACCUR = 0.001;

    private static int vecCount = 0;
    private static int poseCount = 0;
    private static int tagCount = 0;

    private static boolean countPackets() {
        return (double) vecCount / PACKETS_SENT >= SUCCESS_RATIO
                && (double) poseCount / PACKETS_SENT >= SUCCESS_RATIO
                && (double) tagCount / PACKETS_SENT >= SUCCESS_RATIO;
    }

    private static void endCondition(NetworkingServer server) {
        if (countPackets()) {
            server.stop();
            System.out.println("test passed");
        }
    }

    private static boolean expectDouble(double d1, double d2) {
        return Math.abs(d2 - d1) < FLOAT_ACCUR;
    }

    private static boolean expectVec(Translation3d v1, Translation3d v2) {
        return expectDouble(v1.getX(), v2.getX()) && expectDouble(v1.getY(), v2.getY())
                && expectDouble(v1.getZ(), v2.getZ());
    }

    public static void main(String[] args) {
        NetworkingServer server = new NetworkingServer(PORT);

        server.start();

        server.subscribe("vec", (NetworkingCall<Translation3d>) (Translation3d vec) -> {
            if (expectVec(vec, new Translation3d(0, 1, 2))) {
                vecCount++;
                endCondition(server);
            }
        });

        server.subscribe("pose", (NetworkingCall<NetworkingPose>) (NetworkingPose pose) -> {
            if (expectVec(pose.position, new Translation3d(0, 1, 2))
                    && expectVec(pose.rotation, new Translation3d(0, 1, 2))) {
                poseCount++;
                endCondition(server);
            }
        });

        server.subscribe("tag", (NetworkingCall<NetworkingTag>) (NetworkingTag tag) -> {
            if (expectVec(tag.position, new Translation3d(3, 4, 5))
                    && expectVec(tag.rotation, new Translation3d(6, 7, 8)) && tag.id == 2) {
                tagCount++;
                endCondition(server);
            }
        });

        // Running the client executable
        server.onStart(() -> {
            try {
                ProcessBuilder builder = new ProcessBuilder("./bin/test/util/test_client_exec");
                builder.redirectErrorStream(true);
                builder.inheritIO();
                builder.start();
            } catch (Exception error) {
                System.out.println(error);
            }
        });
    }
}