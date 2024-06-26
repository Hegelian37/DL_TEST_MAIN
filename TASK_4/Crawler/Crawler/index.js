/*'use strict';
var http = require('http');
var port = process.env.PORT || 1337;*/

/*http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/plain' });
    res.end('Hello World\n');
}).listen(port);*/

const axios = require("axios");
const cheerio = require("cheerio");
const { Queue } = require('queue-typescript');

const WIKIPEDIA_BASE_URL = 'https://en.wikipedia.org';
const TARGET_PAGE = 'https://en.wikipedia.org/wiki/Adolf_Hitler';
const MAX_HOPS = 6;

const getWikipediaLinks = async (url) => {
    try
    {
        const { data } = await axios.get(url);
        const $ = cheerio.load(data);
        const links = [];
        $('a[href^="/wiki/"]').each((_, element) => {
            const href = $(element).attr('href');
            if (!href.startsWith('/wiki/Special:') && !href.startsWith('/wiki/Help:') && !href.startsWith('/wiki/Main_Page') && !href.includes(':'))
            {
                const fullUrl = new URL(href, WIKIPEDIA_BASE_URL).href;
                links.push(fullUrl);
            }
        });
        return links;
    }
    catch (error)
    {
        console.error(`Error fetching ${url}:`, error.message);
        return [];
    }
};

const findNearestLinksToHitler = async (url) => {
    try
    {
        const { data } = await axios.get(url);
        const $ = cheerio.load(data);
        const paragraphs = $('p').toArray();

        for (const paragraph of paragraphs)
        {
            const paragraphText = $(paragraph).text();
            if (paragraphText.includes('Hitler'))
            {
                const links = [];
                $(paragraph).find('a[href^="/wiki/"]').each((_, element) => {
                    const href = $(element).attr('href');
                    if (!href.startsWith('/wiki/Special:') && !href.startsWith('/wiki/Help:') && !href.startsWith('/wiki/Main_Page') && !href.includes(':'))
                    {
                        const fullUrl = new URL(href, WIKIPEDIA_BASE_URL).href;
                        links.push(fullUrl);
                    }
                });
                return links.reverse();  // Prioritize the last link found first
            }
        }
        return [];
    }
    catch (error)
    {
        console.error(`Error fetching nearest links to Hitler from ${url}:`, error.message);
        return [];
    }
};

const bfsFindTarget = async (startUrl) => {
    cust_depth = 0;
    const visited = new Set();
    const queue = new Queue();
    queue.enqueue({ url: startUrl, depth: 0, path: [startUrl] });

    while (queue.length > 0)
    {
        const { url, depth, path } = queue.dequeue();

        if (visited.has(url))
        {
            continue;
        }

        visited.add(url);
        //console.log(`Visiting: ${url}`);
        cust_depth++;

        if (cust_depth >= MAX_HOPS)
        {
            //console.log("Hitler not found.");
            return null;
        }

        const nearestLinks = await findNearestLinksToHitler(url); //first check for nearest links to Hitler in the text

        if (nearestLinks.length > 0)
        {
            for (const link of nearestLinks)
            {
                if (link === TARGET_PAGE)
                {
                    const fullPath = [...path, link].join(' -> ');
                    //console.log(`Found Adolf Hitler's page: ${fullPath}`);
                    return fullPath;
                }
                if (!visited.has(link))
                {
                    queue.enqueue({ url: link, depth: depth + 1, path: [...path, link] });
                }
            }
        }

        // If no nearest links are found, process all unvisited links on the page
        const links = await getWikipediaLinks(url);
        for (const link of links)
        {
            if (link === TARGET_PAGE)
            {
                const fullPath = [...path, link].join(' -> ');
                //console.log(`Found Adolf Hitler's page: ${fullPath}`);
                return fullPath;
            }
            if (!visited.has(link))
            {
                queue.enqueue({ url: link, depth: depth + 1, path: [...path, link] });
            }
        }
    }

    //console.log("Hitler not found.");
    return null;
};

const main = async () => {
    const startArticle = process.argv[2];
    if (!startArticle || !startArticle.startsWith(WIKIPEDIA_BASE_URL))
    {
        console.error('Please enter a valid Wikipedia article URL.');
        return;
    }

    const targetPage = await bfsFindTarget(startArticle);
    if (targetPage)
    {
        console.log(`Path to Hitler page: ${targetPage}`);
    }
    else
    {
        console.log("Hitler not found.");
    }
};

main();
