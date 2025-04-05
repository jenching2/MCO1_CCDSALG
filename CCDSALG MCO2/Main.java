import java.io.*;
import java.util.*;

public class Main {
    static Map<Integer, List<Integer>> graph = new HashMap<>();

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.print("Input file path: ");
        String path = sc.nextLine();
        if (!loadGraph(path)) {
            System.out.println("Failed to load graph.");
            return;
        }

        System.out.println("Graph loaded!");

        while (true) {
            System.out.println("\nMAIN MENU");
            System.out.println("[1] Get friend list");
            System.out.println("[2] Get connection");
            System.out.println("[3] Exit");
            System.out.print("Enter your choice: ");

            String choice = sc.nextLine();

            if (choice.equals("1")) {
                System.out.print("Enter ID of person: ");
                int id = Integer.parseInt(sc.nextLine());
                displayFriendList(id);
            } else if (choice.equals("2")) {
                System.out.print("Enter ID of first person: ");
                int a = Integer.parseInt(sc.nextLine());
                System.out.print("Enter ID of second person: ");
                int b = Integer.parseInt(sc.nextLine());
                displayConnection(a, b);
            } else if (choice.equals("3")) {
                System.out.println("Exiting...");
                break;
            } else {
                System.out.println("Invalid choice.");
            }
        }

        sc.close();
    }

    static boolean loadGraph(String filename) {
        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {
            String[] firstLine = br.readLine().trim().split(" ");
            int n = Integer.parseInt(firstLine[0]);
            int e = Integer.parseInt(firstLine[1]);

            for (int i = 0; i < n; i++) {
                graph.put(i, new ArrayList<>());
            }

            String line;
            while ((line = br.readLine()) != null) {
                String[] parts = line.trim().split(" ");
                int a = Integer.parseInt(parts[0]);
                int b = Integer.parseInt(parts[1]);
                graph.get(a).add(b);
                graph.get(b).add(a); // undirected graph
            }
            return true;
        } catch (IOException e) {
            System.out.println("Error reading file: " + e.getMessage());
            return false;
        }
    }

    static void displayFriendList(int id) {
        if (!graph.containsKey(id)) {
            System.out.println("ID not found in the network.");
        } else {
            List<Integer> friends = graph.get(id);
            System.out.println("Person " + id + " has " + friends.size() + " friends!");
            System.out.println("List of friends: " + friends.toString().replaceAll("[\\[\\],]", ""));
        }
    }

    static void displayConnection(int start, int end) {
        if (!graph.containsKey(start) || !graph.containsKey(end)) {
            System.out.println("One or both IDs not found in the network.");
            return;
        }

        Queue<Integer> queue = new LinkedList<>();
        Map<Integer, Integer> parent = new HashMap<>();
        Set<Integer> visited = new HashSet<>();

        queue.add(start);
        visited.add(start);
        parent.put(start, -1);

        while (!queue.isEmpty()) {
            int current = queue.poll();
            if (current == end) break;

            for (int neighbor : graph.get(current)) {
                if (!visited.contains(neighbor)) {
                    visited.add(neighbor);
                    parent.put(neighbor, current);
                    queue.add(neighbor);
                }
            }
        }

        if (!parent.containsKey(end)) {
            System.out.println("Cannot find a connection between " + start + " and " + end);
        } else {
            List<Integer> path = new ArrayList<>();
            for (int at = end; at != -1; at = parent.get(at)) {
                path.add(at);
            }
            Collections.reverse(path);
            System.out.println("There is a connection from " + start + " to " + end + "!");
            System.out.println(String.join(" -> ", path.stream().map(Object::toString).toArray(String[]::new)));
        }
    }
}
