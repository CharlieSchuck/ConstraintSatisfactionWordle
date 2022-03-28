/*
    Eric Grandizio
    Dictionary Downloader
    3/27/2022
*/

/* ================================================================================================================================ */

import java.io.*;
import java.util.function.Predicate;

/*
    Uses the jsoup library, found at https://jsoup.org/
*/
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;

/* ================================================================================================================================ */

/*
    Reads all the words found online at https://scrabble.merriam.com and writes them to a local file.
*/
public class DictionaryDownloader {

    public static void main(String[] args) throws Exception {
        System.out.println("\n\nGenerating Full Dictionary...");
        generate_all("dictionary.txt");

        System.out.println("\n\nGenerating 5-letter Dictionary...");
        generate_by_length("dictionary5.txt", 5);

        // Example Usage: [Words that start and end with the same letter]
        // generate_if("test.txt", (String word) -> word.charAt(0) == word.charAt(word.length() - 1));
    }

    /*
        Writes all words to the output file.
    */
    public static void generate_all(String filename) throws Exception {
        generate_if(filename, (String word) -> true);
    }

    /*
        Writes all words of the given length to the output file.
    */
    public static void generate_by_length(String filename, int length) throws Exception {
        generate_if(filename, (String word) -> word.length() == length);
    }

    /*
        Given a Filename and a Predicate, writes all words meeting the given condition to the output file.
    */
    public static void generate_if(String filename, Predicate<String> condition) throws Exception {
        BufferedWriter outfile = new BufferedWriter(new FileWriter(String.format("Dictionaries/%s", filename)));
        try {
            for (char letter = 'a'; letter <= 'z'; ++letter) {
                System.out.printf("\nBrowsing '%c' words...\n", letter);

                for (int page = 1; true; ++page) {
                    String url = String.format("https://scrabble.merriam.com/browse/%c/%d", letter, page);
                    //System.out.printf("\nURL: %s\n", url);

                    Document doc = Jsoup.connect(url).get();
                    Elements ul = doc.select("div.entries ul li");
                    for (Element li : ul) {
                        String word = li.text();

                        if (condition.test(word)) {
                            outfile.write(word);
                            outfile.write('\n');
                            //System.out.printf("\t%s\n", word);
                        }
                    }

                    if (doc.select("div.pagination a.next").isEmpty())
                        break;
                }
            }
        } finally {
            outfile.close();
        }
    }

}

/* ================================================================================================================================ */
