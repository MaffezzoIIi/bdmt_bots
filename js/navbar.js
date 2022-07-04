let processScroll = () => {
    let docElem = document.documentElement, 
        docBody = document.body,
        scrollTop = docElem['scrollTop'] || docBody['scrollTop'],
        scrollBottom = (docElem['scrollHeight'] || docBody['scrollHeight'] - window.innerHeight);
        
    let scrollPercent = scrollTop / scrollBottom * 100;

    if (scrollPercent >= 50) {
        scrollPercent = scrollPercent + 20;
    }

    scrollPercent = scrollPercent + '%';

    document.getElementById('progress-bar').style.setProperty('width', scrollPercent);
}

document.addEventListener('scroll', processScroll);