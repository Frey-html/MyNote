import us.codecraft.webmagic.ResultItems;
import us.codecraft.webmagic.Task;
import us.codecraft.webmagic.pipeline.Pipeline;
import java.util.*;
import java.io.*;

public class MyPipeline implements Pipeline {
    public void process(ResultItems items, Task tasks){
        Map<String,Object> map = items.getAll();
        try{
            File f = new File("C:\\Users\\Frey\\Desktop\\MyNovel.txt");
            FileOutputStream fop = new FileOutputStream(f, true);
            String title;
            String text;
            for(int i = 1;i <= MyPageProcessor.maxPageId;i++) {
                title = "title" + String.valueOf(i);
                text = "text" + String.valueOf(i);
                if(map.get(title) != null){
                    System.out.println(map.get(title).toString() + " √");
                    fop.write(map.get(title).toString().getBytes());
                    fop.write(map.get(text).toString().getBytes());
                    fop.flush();
                }
            }
            //debug
            fop.close();
        }catch (IOException e) {
            System.out.print("Exception");
        }
    }
}
