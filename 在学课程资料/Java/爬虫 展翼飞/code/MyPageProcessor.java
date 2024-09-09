import us.codecraft.webmagic.Page;
import us.codecraft.webmagic.Site;
import us.codecraft.webmagic.Spider;
import us.codecraft.webmagic.pipeline.ConsolePipeline;
import us.codecraft.webmagic.processor.PageProcessor;
import java.util.List;

public class MyPageProcessor implements PageProcessor {

    // 部分一：抓取网站的相关配置，包括编码、抓取间隔、重试次数等
    private Site site = Site.me().setRetryTimes(3).setSleepTime(3);

    //记录文章页的最大id，便于pipeline输出时遍历
    static public int maxPageId;

    @Override
    // process是定制爬虫逻辑的核心接口，在这里编写抽取逻辑
    public void process(Page page) {
        // 部分二：定义如何抽取页面信息，并保存下来

        //区分列表页和文章页
        if(page.getUrl().regex("http://www.jianlaixiaoshuo.com/book/").match()){
            //获取当前文章页id
            String id = page.getUrl().regex("[1-9]\\d*").toString();
            //更新最大页id
            if(Integer.parseInt(id) > maxPageId) maxPageId = Integer.parseInt(id);
            //存储该页标题
            page.putField("title"+id, page.getHtml().xpath("//h1/text()").toString() + "\n");
            //获取该页文本，分段，加换行并存储
            List<String> text = page.getHtml().xpath("//div[@id='BookText']/p/text()").all();
            String textString = "";
            for(int i = 0;i < text.size();i++){
                textString = textString + text.get(i) + "\n";
            }
            page.putField("text"+id, textString);
        }else{
            //将列表页的所有文章链接加入请求队列
            page.addTargetRequests(page.getHtml().links().regex("http://www.jianlaixiaoshuo.com/book/[1-9]\\d*.html").all());
            maxPageId = 0;
        }
    }

    @Override
    public Site getSite() {
        return site;
    }

    public static void main(String[] args) {

        Spider.create(new MyPageProcessor())
                .addUrl("http://www.jianlaixiaoshuo.com/")
                .addPipeline(new MyPipeline())
                //开启1个线程抓取
                .thread(1)
                //启动爬虫
                .run();
        //System.out.println("end");
    }
}