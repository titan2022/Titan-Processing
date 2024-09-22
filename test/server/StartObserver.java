
public class StartObserver {
    private StartCall call;

    public StartObserver(StartCall call) {
        this.call = call;
    }

    public void start() {
        call.start();
    }
}
