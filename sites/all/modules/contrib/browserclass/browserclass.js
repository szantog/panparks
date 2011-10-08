
(function ($) {
  var BrowserClass = {
    init: function() {
      this.agent = navigator.userAgent.toLowerCase();
      this.browser = this.checkBrowser();
      this.platform = this.checkPlatform();
      this.classes = this.browser.concat(this.platform);
    },

    checkBrowser: function() {
      var classes = Array();
      var matches = Array();
      var aresult = '';
      var aversion = '';
      var resultant = '';

      if (this.agent.match(/msie/)) {
        classes.push('ie');

        reg_res = this.agent.match(/.*msie ([0-9]*)\..*/);
        classes.push('ie' + reg_res[1]);

      }

      if (this.agent.match(/opera/)) {
        classes.push('opera');
        
        aresult = this.stristr(this.agent, 'version').split('/');
        if(aresult[1]) {
          aversion = aresult[1].split(' ');
          classes.push('opera' + this.clearVersion(aversion[0]));
        }
      }

      // Chrome is send safari header too
      if (this.agent.match(/chrome/)) {
        classes.push('chrome');
        
        aresult = this.stristr(this.agent, 'chrome').split('/');
        aversion = aresult[1].split(' ');
        classes.push('chrome' + this.clearVersion(aversion[0]));
        
      } else if (this.agent.match(/safari/)) {
        classes.push('safari');
        aresult = this.stristr(this.agent, 'version').split('/');
        
        if(aresult[1]) {
          aversion = aresult[1].split(' ');
          classes.push('safari' + this.clearVersion(aversion[0]));
        }
      }

      if (this.agent.match(/netscape/)) {
        classes.push('netscape');
        
        matches = this.agent.match(/navigator\/([^ ]*)/);
        if (matches) {
          classes.push('netscape' + this.clearVersion(matches[1]));
        }
        else {
          matches = this.agent.match(/netscape6?\/([^ ]*)/);
          if (matches) {
            classes.push('netscape' + this.clearVersion(matches[1]));
          }
        }
      }

      if (this.agent.match(/firefox/)) {
        classes.push('ff');
        matches = this.agent.match(/firefox[\/ \(]([^ ;\)]+)/);
        if(matches) {
          classes.push('ff' + this.clearVersion(matches[1]));
        }
      }

      if (this.agent.match(/konqueror/)) {
        classes.push('konqueror');
        
        aresult = this.stristr(this.agent, 'konqueror').split(' ');
        aversion = aresult[0].split('/');
        classes.push('konqueror' + this.clearVersion(aversion[1]));
      }

      if (this.agent.match(/dillo/)) {
        classes.push('dillo');
      }

      if (this.agent.match(/chimera/)) {
        classes.push('chimera');
      }

      if (this.agent.match(/beonex/)) {
        classes.push('beonex');
      }

      if (this.agent.match(/aweb/)) {
        classes.push('aweb');
      }

      if (this.agent.match(/amaya/)) {
        classes.push('amaya');
      }

      if (this.agent.match(/icab/)) {
        classes.push('icab');
      }

      if (this.agent.match(/lynx/)) {
        classes.push('lynx');
      }

      if (this.agent.match(/galeon/)) {
        classes.push('galeon');
      }

      if (this.agent.match(/opera mini/)) {
        classes.push('operamini');
        
        resultant = this.stristr(this.agent, 'opera mini');
        if(resultant.match('/\//')) {
          aresult = resultant.split('/');
          aversion = aresult[1].split(' ');
          classes.push('operamini' + this.clearVersion(aversion[0]));
        }
        else {
          aversion = this.stristr(resultant, 'opera mini').split(' ');
          classes.push('operamini' + this.clearVersion(aversion[1]));
        }
      }

      return classes;
    },

    checkPlatform: function() {
      var classes = Array();

      if (this.agent.match(/windows/)) {
        classes.push('win');
      }

      if (this.agent.match(/ipad/)) {
        classes.push('ipad');
      }

      if (this.agent.match(/ipod/)) {
        classes.push('ipod');
      }

      if (this.agent.match(/iphone/)) {
        classes.push('iphone');
      }

      if (this.agent.match(/mac/)) {
        classes.push('mac');
      }

      if (this.agent.match(/android/)) {
        classes.push('android');
      }

      if (this.agent.match(/linux/)) {
        classes.push('linux');
      }

      if (this.agent.match(/nokia/)) {
        classes.push('nokia');
      }

      if (this.agent.match(/blackberry/)) {
        classes.push('blackberry');
      }

      if (this.agent.match(/freebsd/)) {
        classes.push('freebsd');
      }

      if (this.agent.match(/openbsd/)) {
        classes.push('openbsd');
      }

      if (this.agent.match(/netbsd/)) {
        classes.push('netbsd');
      }

      return classes;
    },
    
    clearVersion: function(version) {
      version = version.replace('/[^0-9,.,a-z,A-Z-]/', '');
      var find = (version + '').indexOf('.');
      return version.substr(0, find);
    },
    
    stristr: function(haystack, needle, bool) {
      // Finds first occurrence of a string within another, case insensitive  
      // 
      // version: 1103.1210
      // discuss at: http://phpjs.org/functions/stristr
      // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
      // +   bugfxied by: Onno Marsman
      // *     example 1: stristr('Kevin van Zonneveld', 'Van');
      // *     returns 1: 'van Zonneveld'
      // *     example 2: stristr('Kevin van Zonneveld', 'VAN', true);
      // *     returns 2: 'Kevin '
      var pos = 0;

      haystack += '';
      pos = haystack.toLowerCase().indexOf((needle + '').toLowerCase());
      if (pos == -1) {
          return false;
      } else {
          if (bool) {
              return haystack.substr(0, pos);
          } else {
              return haystack.slice(pos);
          }
      }
    }

  };

  BrowserClass.init();
  $('body').addClass(BrowserClass.classes.join(' '));
})(jQuery);